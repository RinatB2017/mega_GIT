	/* Adressing modes */

	void inline absolute();
	void inline absoluteX();
	void inline absoluteY();
	void inline accumulator();
	void inline immediate();
	void inline implied(){ // does nothing 
	}
	void inline indexedX(); //(indirect, X)
	void inline indirect();
	void inline indirectY();
	void inline relative();
	void inline zeropage();
	void inline zeropageX();
	void inline zeropageY();


	
	/* OPCODES */
	
	void inline ADC();
	void inline BRK();
	void inline SBC();
	void inline NOP();
	void inline KIL();

	void inline AND();


	void inline CMP();
	void inline CPX();
	void inline CPY();


	void inline LDA();
	void inline LDX();
	void inline LDY();

	void inline LSR();
	void inline ROL();
	void inline ROR();
	void inline ASL();
	
	
	void inline ORA();

	void inline PHA();
	void inline PHP();
	void inline PLA();
	void inline PLP();


	void inline BCC();
	void inline BCS();
	void inline BEQ();

	void inline BIT();

	void inline BMI();
	void inline BNE();
	void inline BPL();
	void inline BVC();
	void inline BVS();


	void inline CLD();
	void inline CLI();
	void inline CLC();
	void inline CLV();

	void inline SEC();
	void inline SED(); //not used in NES
	void inline SEI();

	void inline STA();
	void inline STX();
	void inline STY();

	void inline TAX();
	void inline TAY();
	void inline TSX();
	void inline TXA();
	void inline TXS();
	void inline TYA();

	void inline DEC();
	void inline DEX();
	void inline DEY();

	void inline EOR();
	void inline INC();
	void inline INX();
	void inline INY();

	void inline JMP();
	void inline JSR();
	void inline RTI();
	void inline RTS();
