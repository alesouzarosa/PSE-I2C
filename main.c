/*
 * File:   main.c
 * Author: ale
 *
 * Created on September 28, 2020, 6:38 PM
 */


/* REGISTRADORES
 * SSPSTAT: SMP, CKE, D/Ã, P, S, R_W, UA, BF
 * 
 * SSPCON1: WCOL, SSPOV, SSPEN, CKP, SSPM3, SSPM2, SSPM1, SSPM0
 *
 * SSPCON2: GCEN, ACKSTAT ,ACKDT, RCEN, PEN, RSEN, SEN  
 * 
 * 
 * 
 
 
 */

/* portas
 * TRIS data direction register 
 * PORT read the levels on the pin of the device
 * LAT Data latch register
 */

/* TRIS C
 *
 */


/* operadores binaroios
 * &   and
 * |   or
 * ^   xor
 * ~   complemento de 1
 * >> desloc direita
 * << desloc esquerda 
 */


#include <xc.h>




// configura o pic para ser o master I2C
void Master_I2C(void)
{
   TRISC = ((TRISC * 0b11100111)| 0b00011000);         //configura pinos SDA e SCL como entrada 
   SSPCON1 =  0b00101000;   //all = 0; SSPEN,SSPM=1, PEN 1000 modo mestre, clock = FOSC/[4 * (SSPADD + 1)]]
   SSPADD = 19;
   SSPSTAT = 0b10000000; // SMP = slow control
}


char I2C_livre(void)
{
   if(SSPSTATbits.R_W) return 0;     // verfica se há transmissaõ em progresso
      if(SSPCON2bits & 0x1f);          // 0b00011111 retorna 0 se ouver algum evento de transmussai em andamento
         else return 1;
}

void I2C_start(void)
{
   SSPCON2bits.SEN = 1;       // start condicional enable
   while(SSPCON2bits.SEN);    // aguarda terminat a condição start
}

void I2C_restart(void)
{
   SSPCON2bits.RSEN = 1;      //inicia o restart 
   while(SSPCON2bits.RSEN);   //aguarda o restart 
}

void I2C_transmite(char I2C_dado)
{
   SSPBUF = I2C_dado;         // carrega o byte de dados no registrador
   while(SSPSTATbits.BF);     // aguarda o buff esvaziar 
   while(SSPSTATbits.R_W);    //
}

char I2C_teste_ACK(void)
{
   if(!SSPCON2bits.ACKSTAT) return 1; 
   else return 0;
}

void I2C_stop(void)
{
   SSPCON2bits.PEN = 1;       // inicia a condição de stop
   while(SSPCON2bits.PEN);    // aguarda a condição de stop concluit
}


char I2C_escrita(unsigned int I2C_end, char I2C_dado)
{
   char x;
   x = I2C_livre();
   if(x==0)
   {
      I2C_stop();
      return -1;
   }
   else
   {
      I2C_start();
      I2C_end << =1; // rotaciona endereco para esquerda
      I2C_transmite(I2C_end);
      if(I2C_teste_ACK())
      {
         I2C_stop();
         return -1;
      }
      I2C_transmite(I2C_dado);
      if(!I2C_teste_ACK())
      {
         I2C_stop();
         return -1;
      }
      I2C_stop();
      return 0;
   }

}




void main(void)
{
   return;
}
