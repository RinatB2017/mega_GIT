
#include "cpu2.h" //if not included, MSVC will go bonkers, g++ is still all about love though



#define ACCUMULATOR_HAXX operand_accumulator = false; A = value

	void M6502::absolute()
	{
	


		adress = MAKE_WORD(memory->ReadMemory(PC+1), memory->ReadMemory(PC+2));

		increment = 3;
	}

	void M6502::absoluteX()
	{

                absolute();

                bool page_cross = (((adress & 0xFF) + X) > 0xFF) ? 1 : 0;


#ifdef M6502_EXTRA_CYCLES
                this->check_extra_cycles_abs_x(adress, this->opcode);
#endif

#ifdef ENABLE_6502_DUMMY_READS
     //check whether we're about to cross a page



#else
    adress = (adress + X) & 0xFFFF;

#endif

                increment = 3;
	}

	void M6502::absoluteY()
	{
		absolute();
#ifdef M6502_EXTRA_CYCLES
                this->check_extra_cycles_abs_y(adress, this->opcode);
#endif
		adress = (adress + Y) & 0xFFFF;	
		increment = 3;
	}

	void M6502::accumulator()
	{
		operand_accumulator = true;

		increment = 1;
	}
	void M6502::immediate()
	{
		increment = 2;
		adress = PC + 1;
	}

	void M6502::indexedX()
	{
                byte h,l;
                byte temp = memory->ReadMemory(PC+1);




                h = memory->ReadMemory((temp + X) & 0xFF);

                //changed this to compensate for the indirection bug

                l = memory->ReadMemory((((temp+1)&0xFF) + X) & 0xFF);

                adress = MAKE_WORD(h,l);
		increment = 2;
		
	}

	void M6502::indirect() //only used for indirection jumps... 
	{

		bool check_jmp_bug_6502 = ( (memory->ReadMemory(PC) == 0x6c) && (memory->ReadMemory(PC+1) ==0xFF)) ? 1:0;

		/* if we've got an indirect jump on a page boundary, the second byte won't be read from the next page, as expected,
		but instead from 0x00 of the page we're on... 6502, tsk tsk :-) 
		this was rather frustrating :-), but as I'm rather bad a looking in documentation it didn't strike me until late :-O
		*/

			byte hx1, hx2;
			hx1 = memory->ReadMemory(PC+1);
			hx2 = memory->ReadMemory(PC+2);
		
			if(!check_jmp_bug_6502)
			adress = MAKE_WORD(
				memory->ReadMemory(MAKE_WORD(hx1, hx2)), 
			
				memory->ReadMemory(MAKE_WORD(hx1, hx2)+1)
				);
			else //we caught the jump bug ^_^
				adress = MAKE_WORD(memory->ReadMemory(MAKE_WORD(hx1, hx2)),
				memory->ReadMemory(MAKE_WORD(0x00, hx2)));

		
		increment = 3;
	}

	void M6502::indirectY()
	{

		byte zp = memory->ReadMemory(PC+1);


		
		//bool wrap_bug = (memory->ReadMemory(zp) == 0xFF) ? 1:0;

		//if(!wrap_bug)
		
			adress = MAKE_WORD(memory->ReadMemory(zp), memory->ReadMemory( (zp+1)&0xff));
#ifdef M6502_EXTRA_CYCLES
                this->check_extra_cycles_ind_y(adress, this->opcode);
#endif
                adress = (adress + Y)&0xFFFF;
		increment = 2;

	}

	void M6502::relative()
	{
		byte t = memory->ReadMemory(PC+1);

		
		adress = PC + 2;
		if(t & 0x80)
                     {
                    #ifdef M6502_EXTRA_CYCLES
                          this->check_extra_cycles_branching(adress,(signed short) (t-0x100));
                    #endif
                        adress += (t-0x100);
                    }
                    else
                    {
                            #ifdef M6502_EXTRA_CYCLES
                          this->check_extra_cycles_branching(adress, t);
                           #endif
			adress += t;
                    }
		increment = 2;
	}

	void M6502::zeropage()
	{
		adress = MAKE_WORD(memory->ReadMemory(PC+1), 0x00);
		increment = 2;

	}

	void M6502::zeropageX()

	{
	
		byte zp = memory->ReadMemory(PC+1);
		zp = (zp + X) & 0xFF;
		adress = MAKE_WORD(zp, 0x00);
		increment = 2;

	}

	void M6502::zeropageY()
	{
	
		byte zp = memory->ReadMemory(PC+1);
	
		zp = (zp + Y) & 0xFF;
	
		adress = MAKE_WORD(zp, 0x00);
	
		increment = 2;
	
	}

	void M6502::implied()
	{

		increment = 1;

	}












	/* Mnemonics */



	void M6502::ADC()
	{

            word temp;
            value = memory->ReadMemory(adress);
            temp = A + value + flags.members.C;

            flags.members.Z = (temp == 0 || temp==0x100) ? 1:0;
            flags.members.N = (temp & 0x80 ) ? 1:0;
            flags.members.V = (!((A ^ value) & 0x80) && ((A^temp) & 0x80))? 1:0;
            flags.members.C = (temp > 0xFF) ? 1:0;
            A = temp &0xFF;



                /*word temp = 0;
		value = memory->ReadMemory(adress);
                temp = A + value + ((flags.members.C == 1)?1:0);
		flags.members.C = (temp > 255)?1:0;

		flags.members.V = (temp & 0xff)?1:0;
		flags.members.Z = (( temp & 0xFF) == 0) ? 1:0;
		flags.members.N = ((temp & 0x80) == 0x80) ? 1:0;

		
                A = (temp & 0xFF);
*/



		/* ADC */
	


		/*
		 carry_flag = valueholder32 > 255;
  278             overflow_flag = valueholder32 > 255;
  279             zero_flag = (valueholder32 & 0xff) == 0;
  280             sign_flag = (valueholder32 & 0x80) == 0x80;
  281 */

	
	}

	void M6502::BRK()
	{

            PC +=2;
            PUSH(MSB(PC));
            PUSH(LSB(PC));

            flags.members.B = 1; //set it before pushing SR to stack
            PUSH(flags.SR);
            flags.members.B = 0;

            flags.members.I = 1; //make sure we don't get another interrupt while we're in this one :-)




            PC = MAKE_WORD(memory->ReadMemory(0xFFFE), memory->ReadMemory(0xFFFF));
            branch = true;

		
	}
	void M6502::SBC()
	{

            /*after fiddling for a long time, the easiest way to implement SBC is just to
              to do ADC (value^0xFF) :). passes various tests now :) YAAAY! bug-free CPU emulation :D :D :D :D :D (probably not though :c)*/

            word temp;
            value = (memory->ReadMemory(adress) ^ 0xFF);
            temp = A + value + flags.members.C;

            flags.members.Z = (temp == 0 || temp==0x100) ? 1:0;
            flags.members.N = (temp & 0x80 ) ? 1:0;
            flags.members.V = (!((A ^ value) & 0x80) && ((A^temp) & 0x80))? 1:0;
            flags.members.C = (temp > 0xFF) ? 1:0;
            A = temp &0xFF;






/* //old and buggy implementation (fails blarggs' offical opcode test, but passes NEStest and NEStress ^;c)
	unsigned int temp;
	value = memory->ReadMemory(adress);
	temp = A - value - (flags.members.C ? 0 : 1);
	
        if(!temp & 0xFF)

                flags.members.Z = 1;
	else
		flags.members.Z = 0;

     //   flags.members.Z = (temp == 0 || temp>0xFF)? 1:0;
	flags.members.N = (temp & 0x80) ? 1:0;
	if(  ((A ^temp) & 0x80) && ((A ^ value) & 0x80))
	
		flags.members.V = 1;
	else
		flags.members.V = 0;
	
	if(temp < 0x100)
		flags.members.C = 1;
	else
		flags.members.C = 0;

	A =(byte) temp &0xFF;
*/



	}
	void M6502::NOP()
	{
		
	}
	void M6502::KIL()
	{
	
	}

	void M6502::AND()
	{
		value = memory->ReadMemory(adress);
		A = A & value;
		flags.members.N = GET_BIT(A,7) ? 1:0;
		flags.members.Z = (A==0) ? 1:0;	
	}


	void M6502::CMP()
	{

                value = (byte)(A - memory->ReadMemory(adress));
		flags.members.N = GET_BIT(value, 7) ? 1:0;
		flags.members.C = (A >= memory->ReadMemory(adress))? 1:0;
		flags.members.Z = (value == 0)? 1:0;
	
	}
	void M6502::CPX()
	{
                value = (byte)(X - memory->ReadMemory(adress));
		flags.members.N = GET_BIT(value, 7) ? 1:0;
		flags.members.C = (X >= memory->ReadMemory(adress))? 1:0;
		flags.members.Z = (value == 0)? 1:0;
	}
	void M6502::CPY()
	{

                value = (byte)(Y - memory->ReadMemory(adress));
		flags.members.N = GET_BIT(value, 7) ? 1:0;
		flags.members.C = (Y >= memory->ReadMemory(adress))? 1:0;
//		flags.members.C = (value < 0x100) ? 1:0;
		flags.members.Z = (value == 0)? 1:0;
	
	}


	void M6502::LDA()
	{
		A = memory->ReadMemory(adress);
		flags.members.N = GET_BIT(A, 7) ? 1:0;
		flags.members.Z = (A==0) ? 1:0;

	
	}
	void M6502::LDX()
	{
	
		X = memory->ReadMemory(adress);

		flags.members.N = GET_BIT(X,7)? 1:0;
		flags.members.Z = (X==0) ? 1:0;
	}
	void M6502::LDY()
	{
	
		Y = memory->ReadMemory(adress) & 0xff;

		flags.members.N = GET_BIT(Y, 7) ? 1:0;
		flags.members.Z = (Y==0) ? 1:0;

	}

	void M6502::LSR()
	{
		
		if(!operand_accumulator)
			value = memory->ReadMemory(adress);
		else
			value = A;


		flags.members.N = 0;
		flags.members.C = GET_BIT(value, 0) ? 1:0 ;
		value = (value >> 1);// & 0x7F;
		flags.members.Z = (value == 0)? 1:0;

		if(!operand_accumulator)
			memory->WriteMemory(adress, value);
		else
			A = value;
		
	

	}

	void M6502::ROL()
	{
		
		
		
			byte tmp;

		if(operand_accumulator)
			value = A;
		else
			value = memory->ReadMemory(adress);

		tmp = value;


			
                value = (value<<1) & 0xFE;
		value = value | flags.members.C;
		flags.members.C = GET_BIT(tmp, 7) ? 1:0;
		flags.members.Z = (value==0)?1:0;
		flags.members.N = GET_BIT(value,7) ? 1:0;
	


		if(operand_accumulator)
			A = value;
		else
			memory->WriteMemory(adress, value);

	}
	void M6502::ROR()
	{
		byte tmp;

		if(operand_accumulator)
			value = A;
		else
			value = memory->ReadMemory(adress);


                tmp = value;

                value = (value >> 1 ) & 0xFF;
                value = value | ((flags.members.C ==1)? 0x80 : 0x00);
                flags.members.C = GET_BIT(tmp, 0);
                flags.members.Z = (value == 0) ? 1:0;
                flags.members.N = GET_BIT(value, 7) ? 1:0;

		if(operand_accumulator)
			A = value;
		else
			memory->WriteMemory(adress, value);

		
		
	}
	void M6502::ASL()
	{
		if(operand_accumulator)
			value = A;
		else
			value = memory->ReadMemory(adress);

		flags.members.C = GET_BIT(value, 7) ? 1:0;

		value = (value << 1);// & 0xFE;
		flags.members.N = GET_BIT(value, 7) ? 1:0;
		flags.members.Z = (value == 0)? 1:0;



		if(operand_accumulator)
			A = value;
		else
			memory->WriteMemory(adress, value);
		
	}
	
	
	void M6502::ORA()
	{
		A = A | memory->ReadMemory(adress);
		flags.members.N = GET_BIT(A, 7) ? 1:0;
		flags.members.Z = (A == 0)? 1:0;

	}

	void M6502::PHA()
	{	

		PUSH(A);

	
	}
	void M6502::PHP()
	{
                flags.members.B = 1; //always set for PHP and BRK
                flags.members.unused = 1;

                PUSH(flags.SR );
	}
	void M6502::PLA()
	{
                A = POP() ;
                flags.members.N = GET_BIT(A, 7)?1:0;
		flags.members.Z = (A==0)? 1:0;
                //flags.members.B = 0; //makes the shit pass nestest.nes?
	
	}
	void M6502::PLP()
	{
                flags.SR = POP() | 0x20;

              //  flags.members.B = 0;
                 //  flags.SR = POP();
                   //flags.members.D |= 0x1;
	}


	void M6502::BCC()
	{
		if(flags.members.C == 0)
		{
			PC = adress;
			branch = true;
#ifdef M6502_EXTRA_CYCLES
                        extra_cycles = 1;
#endif
		}
		else
			branch= false;

	}

	void M6502::BCS()
	{
		if(flags.members.C == 1)
		{
			PC = adress;
#ifdef M6502_EXTRA_CYCLES
                        extra_cycles = 1;
#endif
			branch = true;
		}
		else
			branch= false;
	}

	void M6502::BEQ()
	{
		if(flags.members.Z == 1)
		{
			PC = adress;
#ifdef M6502_EXTRA_CYCLES
                        extra_cycles = 1;
#endif
			branch = true;
		}
		else
			branch= false;
	}

	void M6502::BIT()
	{
                byte mem = memory->ReadMemory(adress);
                value = A & mem;
                flags.members.N = GET_BIT(mem, 7) ? 1:0;
                flags.members.V = GET_BIT(mem, 6) ? 1:0;


                flags.members.Z = (value == 0)? 1:0;
	}

	void M6502::BMI()
	{
		if(flags.members.N == 1)
		{
			PC = adress;
#ifdef M6502_EXTRA_CYCLES
                        extra_cycles = 1;
#endif
			branch = true;
		}
		else
			branch= false;

	}
	void M6502::BNE()
	{
		if(flags.members.Z == 0)
		{
			PC = adress;
#ifdef M6502_EXTRA_CYCLES
                        extra_cycles = 1;
#endif
			branch = true;
		}
		else
			branch= false;

		
	}
	void M6502::BPL()
	{
		if(flags.members.N == 0)
		{
			PC = adress;
#ifdef M6502_EXTRA_CYCLES
                        extra_cycles = 1;
#endif
			branch = true;
		}
		else
			branch= false;
	}
	void M6502::BVC()
	{
		if(flags.members.V == 0)
		{
			PC = adress;
#ifdef M6502_EXTRA_CYCLES
                        extra_cycles = 1;
#endif
			branch = true;
		}
		else
			branch = false;
		
	}
	void M6502::BVS()
	{
		if(flags.members.V == 1)
		{
			PC = adress;
#ifdef M6502_EXTRA_CYCLES
                        extra_cycles = 1;
#endif
			branch = true;
		}
		else
			branch = false;


		
	}


	void M6502::CLD()
	{
		flags.members.D = 0; //not used in NES, or so I've been told :-). various CPU-testing roms might check for it through some ugly hacking :-)
	}
	void M6502::CLI()
	{
		flags.members.I = 0;
	
	}
	void M6502::CLC()
	{
		flags.members.C = 0;
	
	}
	void M6502::CLV()
	{
		flags.members.V = 0;
	}

	void M6502::SEC()
	{
		flags.members.C = 1;
	
	}
	void M6502::SED()
	{
		flags.members.D = 1;
	
	} //not used in NES

	void M6502::SEI()
	{
		flags.members.I = 1;
	}

	void M6502::STA()
	{
		memory->WriteMemory(adress, A);
	}
	void M6502::STX()
	{
		memory->WriteMemory(adress, X);
	}
	void M6502::STY()
	{
		memory->WriteMemory(adress, Y);
	
	}

	void M6502::TAX()
	{
		 X = A;
		 flags.members.N = GET_BIT(X,7) ?1:0;
		 flags.members.Z = (X==0)? 1:0;

	}
	void M6502::TAY()
	{
		Y = A;
		flags.members.N = GET_BIT(Y,7)?1:0;
		flags.members.Z = (Y==0)? 1:0;
	
	}
	void M6502::TSX()
	{
	
		X = SP;
		flags.members.N = GET_BIT(X,7)?1:0;
		flags.members.Z = (X==0)? 1:0;
	}
	void M6502::TXA()
	{
		A = X;
		flags.members.N = GET_BIT(A,7)?1:0;
		flags.members.Z = (A==0)? 1:0;
	
	}
	void M6502::TXS()
	{
		SP = X;


	}
	void M6502::TYA()
	{
		A = Y;
		flags.members.N = GET_BIT(A,7)?1:0;
		flags.members.Z = (A==0)? 1:0;

	}

	void M6502::DEC()
	{
		value = memory->ReadMemory(adress);
		value = (value - 1) & 0xFF;
		flags.members.N = GET_BIT(value, 7) ? 1:0;
		flags.members.Z = (value == 0)? 1:0;
		memory->WriteMemory(adress, value);
	
	
	}
	void M6502::DEX()
	{
		X = X - 1;
		flags.members.Z = (X == 0)?1:0;
		flags.members.N = GET_BIT(X, 7) ? 1:0;

	
	}
	void M6502::DEY()
	{
		Y = Y - 1;
		flags.members.Z = (Y == 0)?1:0;
		flags.members.N = GET_BIT(Y, 7) ? 1:0;

	
	}

	void M6502::EOR()
	{
	
		A = A ^ memory->ReadMemory(adress);
		flags.members.N = GET_BIT(A, 7)?1:0;
		flags.members.Z = (A==0)? 1:0;

	}
	void M6502::INC()
	{
		value = memory->ReadMemory(adress);
		value = (value + 1) & 0xFF;
		flags.members.N = GET_BIT(value, 7) ?1:0;
		flags.members.Z = (value == 0) ? 1:0;
		memory->WriteMemory(adress, value);

	}
	void M6502::INX()
	{
		X = X + 1;
		flags.members.Z = ( X == 0)? 1:0;
		flags.members.N = GET_BIT(X, 7) ? 1:0;
	
	}
	void M6502::INY()
	{

		Y = Y + 1;
		flags.members.Z = ( Y == 0)? 1:0;
		flags.members.N = GET_BIT(Y, 7) ? 1:0;
	
	}

	void M6502::JMP()
	{
		branch = true;
		//haffi fix absolute adressing >_<
	
		PC=adress;
	}
	void M6502::JSR()
	{
		PC += 2;
		PUSH(MSB(PC));
		PUSH(LSB(PC));

		PC = adress;
		branch = true;

	}
        #include <iostream>

        void M6502::NMI()
        {

                        PUSH(MSB(PC)); //push PC to stack
                        PUSH(LSB(PC)); //

                        flags.members.B = 0;//always cleared when NMI occurs
                        PUSH(flags.SR); //push status register

                        //read the NMI vector from PRG
                        PC = MAKE_WORD(memory->ReadMemory(0xFFFA), memory->ReadMemory(0xFFFB));



                        //ppu.NMI_occured = true;
                        ppu_2->NMI_occured = true;


                        flags.members.I = 1; //make sure we don't get an interrupt race condition


                        this->cycles += 7; // all interrupts uses 7 cycles

                        //NMI MUSTN'T BRANCH, AS OPPOSED TO THE LAST IMPLEMENTATION ;)
                   //     branch = false; //<--- cause of the infamous PHA-stack-corruption-bug in certain ROMs

        }

	void M6502::RTI()
        {
                flags.SR = POP();

		byte msb, lsb;
		lsb = POP();//lsb
		msb = POP();//msb

                PC = MAKE_WORD(lsb, msb);

               //ppu.NMI_occured = false;
                ppu_2-> NMI_occured = false;


                branch = true;


              //  flags.members.I = 0; //no longer in interrrrrrrupt

	
	}
	void M6502::RTS()
	{
		byte lsb = POP();
		byte msb = POP();
		PC = MAKE_WORD(lsb, msb) + 1;
                branch = true; //since we're modifying PC here, make sure it doesn't get incremented in the opcode-fetch-loop
			
	}
