// DIY Famicom Multitap by takuya matsubara
// for Atmega168/328  5V/16MHz(Arduino Pro/Pro Mini)

#define LEDENABLE 1
#define LED_BIT  (1<<0)
#define LED_PORT PORTB
#define LED_DDR DDRB

#define P1CLK_BIT (1<<3)  
#define P1DAT_BIT (1<<2)  
#define P2CLK_BIT (1<<5)  
#define P2DAT_BIT (1<<4)  
#define P3CLK_BIT (1<<3)  
#define P3DAT_BIT (1<<2)  
#define P4CLK_BIT (1<<5)  
#define P4DAT_BIT (1<<4)  

#define P1P2_PORT PORTB
#define P1P2_PIN PINB
#define P1P2_DDR DDRB

#define P3P4_PORT PORTD
#define P3P4_PIN PIND
#define P3P4_DDR DDRD

#define FC_PORT PORTC
#define FC_PIN PINC
#define FC_DDR DDRC

#define LATCH_BIT (1<<1)
#define LATCH_DDR DDRB
#define LATCH_PORT PORTB

#define MODE_PORT PORTD
#define MODE_PIN  PIND
#define MODE_DDR DDRD
#define MODE_BIT (1<<7)

unsigned char id1;
unsigned char id2;

void p1data(void){
    if(P1P2_PIN & P1DAT_BIT){
      FC_PORT |= P1DAT_BIT; // off
    }else{
      FC_PORT &= ~P1DAT_BIT; // on
    }
}

void p2data(void){
    if(P1P2_PIN & P2DAT_BIT){
      FC_PORT |= P2DAT_BIT; // off
    }else{
      FC_PORT &= ~P2DAT_BIT; // on
    }
}

void p3data(void){
    if(P3P4_PIN & P3DAT_BIT){
      FC_PORT |= P1DAT_BIT; // off
    }else{
      FC_PORT &= ~P1DAT_BIT; // on
    }
}

void p4data(void){
    if(P3P4_PIN & P4DAT_BIT){
      FC_PORT |= P2DAT_BIT; // off
    }else{
      FC_PORT &= ~P2DAT_BIT; // on
    }
}

void id1data(void){
  if(id1 & 0x80){
    FC_PORT &= ~P1DAT_BIT;
  }else{
    FC_PORT |= P1DAT_BIT;
  }
}

void id2data(void){
  if(id2 & 0x80){
    FC_PORT &= ~P2DAT_BIT;
  }else{
    FC_PORT |= P2DAT_BIT;
  }
}

//----------
void setup() {
  FC_DDR |= (P1DAT_BIT + P2DAT_BIT);    // FAMICOM <-- MULTITAP
  FC_PORT |= (P1DAT_BIT + P2DAT_BIT );  //OUTPUT
  FC_DDR &= ~(P1CLK_BIT + P2CLK_BIT);   // FAMICOM --> MULTITAP
  FC_PORT |= (P1CLK_BIT + P2CLK_BIT + LATCH_BIT);  // PULLUP

  P1P2_DDR &= ~(P1DAT_BIT + P2DAT_BIT);  // GAMEPAD --> MULTITAP
  P1P2_PORT |= (P1DAT_BIT + P2DAT_BIT);  // PULLUP
  P1P2_DDR |= (P1CLK_BIT + P2CLK_BIT);   // GAMEPAD <-- MULTITAP
  P1P2_PORT |= (P1CLK_BIT + P2CLK_BIT);  // OUTPUT

  P3P4_DDR &= ~(P3DAT_BIT + P4DAT_BIT);  // GAMEPAD --> MULTITAP
  P3P4_PORT |= (P3DAT_BIT + P4DAT_BIT);  // PULLUP
  P3P4_DDR |= (P3CLK_BIT + P4CLK_BIT);   // GAMEPAD <-- MULTITAP
  P3P4_PORT |= (P3CLK_BIT + P4CLK_BIT);  // OUTPUT

  MODE_DDR &= ~MODE_BIT;  // INPUT
  MODE_PORT |= MODE_BIT; //  PULLUP

  LATCH_DDR |= LATCH_BIT; // OUTPUT
  LATCH_PORT &= ~LATCH_BIT;  // GAMEPAD LATCH

#if LEDENABLE
  LED_DDR |= LED_BIT;
  LED_PORT &= ~LED_BIT;
#endif
}

//------ Downttown nekketsu koushien soreyuke dai undoukai
void loop2(void){
  char loopcnt;
  while(1){
  
    while((FC_PIN & LATCH_BIT)==0); // PS == high待ち
    LATCH_PORT |= LATCH_BIT;  // CONTROLLER LOAD
  #if LEDENABLE
    LED_PORT |= LED_BIT;
  #endif 
    id1 = 0x20;
    id2 = 0x10;
    while((FC_PIN & LATCH_BIT)!=0); // PS == low待ち
    LATCH_PORT &= ~LATCH_BIT;  // START LATCH

    p1data();
    p2data();
//---- player 2
    loopcnt = 8;
    while(loopcnt--){
      while((FC_PIN & P2CLK_BIT)!=0); // low待ち
      P1P2_PORT &= ~P2CLK_BIT;
      while((FC_PIN & P2CLK_BIT)==0); // high待ち
      P1P2_PORT |= P2CLK_BIT;
      p2data();
//    p2data();
    }

//---- player 1
    loopcnt = 8;
    while(loopcnt--){
      while((FC_PIN & P1CLK_BIT)!=0); // low待ち
      P1P2_PORT &= ~P1CLK_BIT;
      while((FC_PIN & P1CLK_BIT)==0); // high待ち
      P1P2_PORT |= P1CLK_BIT;
      p1data();
//    p1data();
    }

//    if((MODE_PIN & MODE_BIT)==0)continue;  // 2P MODE

    p3data();
    p4data();
//---- player 4
    loopcnt = 8;
    while(loopcnt--){
      while((FC_PIN & P2CLK_BIT)!=0){
      } // low待ち
      P3P4_PORT &= ~P4CLK_BIT;
      while((FC_PIN & P2CLK_BIT)==0){
      } // high待ち
      P3P4_PORT |= P4CLK_BIT;
      p4data();
//    p4data();
    }
//---- player 3
    loopcnt = 8;
    while(loopcnt--){
      while((FC_PIN & P1CLK_BIT)!=0){
      } // low待ち
      P3P4_PORT &= ~P3CLK_BIT;
      while((FC_PIN & P1CLK_BIT)==0){
      } // high待ち
      P3P4_PORT |= P3CLK_BIT;
      p3data();
//    p3data();
    }

    id1data();
    id2data();
  //---- ID 2
    loopcnt = 8;
    while(loopcnt--){
      while((FC_PIN & P2CLK_BIT)!=0); // low待ち
      while((FC_PIN & P2CLK_BIT)==0); // high待ち
      id2 <<= 1;
      id2data();
    }
    loopcnt = 8;
    while(loopcnt--){
      while((FC_PIN & P1CLK_BIT)!=0); // low待ち
      while((FC_PIN & P1CLK_BIT)==0); // high待ち
      id1 <<= 1;
      id1data();
    }
  }
}
//----------------
void loop1(void){
  char loopcnt;
  while(1){
    id1 = 0x20;
    id2 = 0x10;
    while((FC_PIN & LATCH_BIT)==0); // PS == high待ち
    LATCH_PORT |= LATCH_BIT;  // controller LOAD
  #if LEDENABLE
    LED_PORT |= LED_BIT;
  #endif 
    while((FC_PIN & LATCH_BIT)!=0); // PS == low待ち
    LATCH_PORT &= ~LATCH_BIT;  // controller LATCH

//---- player 1 / 2
    p1data();
    p2data();
    loopcnt = 8;
    while(loopcnt--){
      while((FC_PIN & P1CLK_BIT)!=0){
      } // low待ち
      P1P2_PORT &= ~P1CLK_BIT;
      while((FC_PIN & P1CLK_BIT)==0){
      } // high待ち
      P1P2_PORT |= P1CLK_BIT;
      p1data();
      p1data();

      while((FC_PIN & P2CLK_BIT)!=0){
      } // low待ち
      P1P2_PORT &= ~P2CLK_BIT;
      while((FC_PIN & P2CLK_BIT)==0){
      } // high待ち
      P1P2_PORT |= P2CLK_BIT;
      p2data();
      p2data();
    }
//    if((MODE_PIN & MODE_BIT)==0)continue;  // 2P MODE

//---- player 3 / 4
    p3data();
    p4data();
    loopcnt = 8;
    while(loopcnt--){
      while((FC_PIN & P1CLK_BIT)!=0){
      }    // low待ち
      P3P4_PORT &= ~P3CLK_BIT;
      while((FC_PIN & P1CLK_BIT)==0){
      } // high待ち
      P3P4_PORT |= P3CLK_BIT;
      p3data();
      p3data();
      while((FC_PIN & P2CLK_BIT)!=0){
      } // low待ち
      P3P4_PORT &= ~P4CLK_BIT;
      while((FC_PIN & P2CLK_BIT)==0){
      } // high待ち
      P3P4_PORT |= P4CLK_BIT;
      p4data();
      p4data();
    }

  //---- ID 1 / 2
    id1data();
    id2data();
    loopcnt = 8;
    while(loopcnt--){
      while((FC_PIN & P1CLK_BIT)!=0); // low待ち
      while((FC_PIN & P1CLK_BIT)==0); // high待ち
      id1 <<= 1;
      id1data();
      while((FC_PIN & P2CLK_BIT)!=0); // low待ち
      while((FC_PIN & P2CLK_BIT)==0); // high待ち
      id2 <<= 1;
      id2data();
    }
#if LEDENABLE
    LED_PORT &= ~LED_BIT;
#endif 
  }  
}

//-----------
void loop() {
  cli();//割り込み禁止
  while((FC_PIN & LATCH_BIT)!=0){
  } // PS == low待ち
  while((FC_PIN & LATCH_BIT)==0){
  } // latch=high待ち
  while(1){
    if((FC_PIN & P2CLK_BIT)==0){  // Player2から先にCLKが来た場合、
      loop2();  //ダウンタウン熱血行進曲　専用
    }
    if((FC_PIN & P1CLK_BIT)==0){
      loop1();  //それ以外
    }
  }
}

