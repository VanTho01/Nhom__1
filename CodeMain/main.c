#define LCD_ENABLE_PIN     PIN_D3           
#define LCD_RS_PIN         PIN_D1                                                   
#define LCD_RW_PIN         PIN_D2                         
#define LCD_DATA4          PIN_D4                                                          
#define LCD_DATA5          PIN_D5                                    
#define LCD_DATA6          PIN_D6                                                      
#define LCD_DATA7          PIN_D7   


// dinh nghia ten cac phim bam
#define KEY0 0
#define KEY1 1
#define KEY2 2
#define KEY3 3
#define KEY4 4
#define KEY5 5
#define KEY6 6
#define KEY7 7
#define KEY8 8
#define KEY9 9
#define KEYSTAR 10
#define KEYHASH 11
#define NOKEY 12

#define KC1 PIN_B2
#define KC2 PIN_B1
#define KC3 PIN_B0
#define KR1 PIN_B4
#define KR2 PIN_B5
#define KR3 PIN_B6
#define KR4 PIN_B7

// khai bao dinh nghia cac trang thai mach
#define STAMAIN 0
#define STAINPUTPASS 1


#define MFRC522_CS         PIN_C0                 
#define MFRC522_SCK        PIN_C1
#define MFRC522_SI         PIN_C2                           
#define MFRC522_SO         PIN_C3              
#define MFRC522_RST        PIN_C4  

#include <main.h>
#include <lcd.c> 
#include<Built_in.h>

int8 AdminPass[6]={1,2,3,4,5,6};

int8 CurrentKey; //bien luu gia tri phim bam
int8 CurrentCodeIndex; //vi tri so hien tai
int8 CurrentPass[6]; // bien luu ma hien tai
int8 CurrentState=STAMAIN; // Bien luu trang thai cua mach hien tai

int8 WrongCount=0; // bien dem so lan sai pass




CHAR UID[6];
char Card1[6]={227,168,219,13,157,0};
char Card2[6]={99,68,229,246,52,0};
char Card3[6]={19,122,196,18,191,0};


void DisplayCardID()
{
          int i;
           printf (LCD_PUTC,"\f");
           lcd_gotoxy(0,0);
           for(i=0;i<3;i++)
           {
              lcd_putc(UID[i]/100+48);
              lcd_putc((UID[i]%100)/10+48);
              lcd_putc(UID[i]%10+48);
              lcd_putc(' ');
           }
           lcd_gotoxy(0,1);
           for(i=3;i<6;i++)
           {
              lcd_putc(UID[i]/100+48);
              lcd_putc((UID[i]%100)/10+48);
              lcd_putc(UID[i]%10+48);
              lcd_putc(' ');
           }
           
          
}


int8 ScanKeyboard(void)
{
 output_low(KC1);
 if(!input(KR1)) 
 { 
  while(!input(KR1));
  delay_ms(10);
  return KEY1;
 }
 
 if(!input(KR2)) 
  { 
   while(!input(KR2));
   delay_ms(10);
   return KEY4;
  }
  
 if(!input(KR3))
 {
  while(!input(KR3));
  delay_ms(10);
  return KEY7;
 }
 
 if(!input(KR4)) 
  {
  while(!input(KR4));
  delay_ms(10);
  return KEYSTAR;
 }
 
 output_high(KC1);
 output_low(KC2);
 
 if(!input(KR1)) 
 { 
  while(!input(KR1));
  delay_ms(10);
  return KEY2;
 }
 
 if(!input(KR2)) 
  { 
   while(!input(KR2));
   delay_ms(10);
   return KEY5;
  }
  
 if(!input(KR3))
 {
  while(!input(KR3));
  delay_ms(10);
  return KEY8;
 }
 
 if(!input(KR4)) 
  {
  while(!input(KR4));
  delay_ms(10);
  return KEY0;
 }
 
 output_high(KC2);
 output_low(KC3);
 
 if(!input(KR1)) 
 { 
  while(!input(KR1));
  delay_ms(10);
  return KEY3;
 }
 
 if(!input(KR2)) 
  { 
   while(!input(KR2));
   delay_ms(10);
   return KEY6;
  }
  
 if(!input(KR3))
 {
  while(!input(KR3));
  delay_ms(10);
  return KEY9;
 }
 
 if(!input(KR4)) 
  {
  while(!input(KR4));
  delay_ms(10);
  return KEYHASH;
 }
 
 output_high(KC3);
 return NOKEY;

}

void OnBuzzer()
{
    output_high(PIN_E0);// Mo Loa
}

void OffBuzzer()
{
    output_Low(PIN_E0);// tat Loa
}

void Beep(int BeepCount) // phattieng pip theo so lan
{
 int i;
 for(i=0;i<BeepCount;i++)
 {
    OnBuzzer();
    delay_ms(200);
    OffBuzzer();  
    delay_ms(200);
 }
}

void OnLedGreen()
{
 output_high(PIN_E1);
}

void OffLedGreen()
{
 output_Low(PIN_E1);
}

void OnLedRed()
{
 output_high(PIN_E2);
}

void OffLedRed()
{
 output_Low(PIN_E2);
}


void DisplayMain()
{
   lcd_putc("\f");
   lcd_putc("KHOA MAT MA-RFID");  
}

void DisplayInputPass()
{
   lcd_putc("\f");
   lcd_putc("  NHAP MAT MA");  
   lcd_gotoxy(1, 2);

}


int8 CheckPass() // ham kiem tra mat ma, ket qua tra ve 0 - mat ma sai, tra ve 1 - pass admin, tra ve 2 - pass phu
{
  int8 PassResult=1; // gia su pass la dung
  int i;
  for(i=0;i<6;i++) // kiem tra pass add min
  {
   if(CurrentPass[i]!=AdminPass[i]) // neu phat hien 1 mat ma sai thi thoat vong lam
   {
     return 0;
     break;
   }
  }
 
  if(PassResult==1) return 1;// neu pass tren dung thi tra ve ket qua admin thoat ham
  
  
}

void ResetInput() // ham reset du lieu dang nhap
{
  CurrentCodeIndex=0;
  CurrentPass[0]=0xFF;
  CurrentPass[1]=0xFF;
  CurrentPass[2]=0xFF;
  CurrentPass[3]=0xFF;
  CurrentPass[4]=0xFF;
  CurrentPass[5]=0xFF;
}
char CheckCard()
{
 char Result;
 int i;
 Result=1;
 for(i=0;i<6;i++)
 {
      if(UID[i]!=Card1[i]){
            return 0;
         }
 }
 for (int i = 0; i < 6; i++) {
        if (UID[i] != Card2[i]) {
            return 0;
        }
    }
   for (int i = 0; i < 6; i++) {
        if (UID[i] != Card3[i]) {
            return 0;
        }
    }
 
 if(Result==1) return 1;


}

void main()
{
   int8 CheckPassResult;
   UNSIGNED int TagType;    
   char CardFound=0;
   SET_TRIS_D(0x00);
   SET_TRIS_E(0x00);
   SET_TRIS_C(0x08);
   Set_tris_B(0xF0);// cai dat port B 0-3 output, 4-7 input
   port_B_pullups(0xFF);   
   output_high(KC1);// dua dua cac chan cot phim len cao
   output_high(KC2);
   output_high(KC3);
   
   OffBuzzer();
   OffLedGreen();
   OffLedRed();
   lcd_init(); // khoi tao LCD
   DisplayMain();
   MFRC522_Init ();
   while(TRUE)
   {
      //TODO: User Code
      
       // phan kim tra phim bam va mat ma
      CurrentKey=Scankeyboard(); // doc ban phim
      if(CurrentKey!=NOKEY) // neu phim tra ve khac NOKEY ( tuc la co bam phim)
      {
        switch(CurrentKey) // kiem tra xem la phim gi
        {
            case KEY0: // Neu la ca phim so
            case KEY1:
            case KEY2:
            case KEY3:
            case KEY4:
            case KEY5:
            case KEY6:
            case KEY7:
            case KEY8:
            case KEY9:
            {
               if(CurrentState==STAMAIN) // neu dang o che do cho hoac che do nhap pass thi cho phep nhap
               {
                 
                 CurrentState=STAINPUTPASS; // cai trang tahi hien tai la nhap pass
                 DisplayInputPass();
              
               }
               else if(CurrentState==STAINPUTPASS)
               {
                 if(CurrentCodeIndex<6) //neu nhap chua du 6 ky tu
                 {
                    CurrentPass[CurrentCodeIndex]=CurrentKey; // luu so vua bam vao mang
                    lcd_putc('*'); // hien thi dau sao cho phim da bam
                    CurrentCodeIndex++;// chuyen sang vi tri tiep theo
                 }
               }
               break;
            }
            case KEYHASH:// Neu bam phim thang
            {
              if(CurrentState==STAINPUTPASS) // neu dang nhap pass de mo
              {
                CheckPassResult=CheckPass(); // kiem tra mat ma nhap vao
                if(CheckPassResult==0) //neu sai pass thi thong bao
                {
                   OnLEDRed();
                   lcd_gotoxy(1, 2);
                   lcd_putc("   SAI MAT MA!  ");
                   Beep(3); // beep loa 3 tieng
                   delay_ms(1000);
                   ResetInput();  // xoa du lieu da nhap
                   DisplayInputPass();
                   OffLEDRed();
                   WrongCount++;
                   if(WrongCount==3)
                   {OnBuzzer();
                      OnLEDRed();
                      lcd_gotoxy(1, 2);
                      lcd_putc("CANH BAO BAO MAT!");
                      while(1);
                   }
                }
                else
                {
                   lcd_gotoxy(1, 2);
                   lcd_putc("  XIN MOI VAO!  ");
                   OnLEDGreen();
                   Beep(1); // beep loa 1 tieng
                   delay_ms(3000);
                   ResetInput();  // xoa du lieu da nhap
                   CurrentState=STAMAIN;
                   DisplayMain();
                   OffLEDGreen();
                   WrongCount=0;
                }
              }
              break;
            }
             
        }
      }
      
       IF (MFRC522_isCard (&TagType)) //Check any card
      {                                           
         //Read ID 
         IF (MFRC522_ReadCardSerial (&UID))             
         {
           CardFound=CheckCard();
           if(CardFound==0)
           {
                   OnLEDRed();
                   lcd_gotoxy(1, 2);
                   lcd_putc("   SAI MAT MA!  ");
                   Beep(3); // beep loa 3 tieng
                   delay_ms(1000);
                   DisplayMain();
                   OffLEDRed();
                   WrongCount++;
                   if(WrongCount==3)
                   {
                      OnBuzzer();
                      OnLEDRed();
                      lcd_gotoxy(1, 2);
                      lcd_putc("CANH BAO BAO MAT!");
                      while(1);
                   }
            
           }
           else
           {
                   lcd_gotoxy(1, 2);
                   lcd_putc("  XIN MOI VAO!  ");
                   OnLEDGreen();
                   Beep(1); // beep loa 1 tieng
                   delay_ms(3000);
                   DisplayMain();
                   OffLEDGreen();
                   WrongCount=0;
           }
           
         }                                      
         
        MFRC522_Halt () ;
      }    
      
   }

}
