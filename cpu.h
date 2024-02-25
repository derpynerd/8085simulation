#include "constants.h"
#include "memory.h"
#pragma once

class CPU  {

    /* Internal Registers */
    Word SP; // Stack pointer
    Word PC; // Program counter
    Byte IR; // Instruction register

    /* Registers */
    Byte A; // Accumulator
    Word WZ; // Temporary registers
    Word BC, DE, HL; // Registers

    /* Flag register */
    Byte S : 1; // Sign flag 
    Byte Z : 1; // Zero flag
    Byte AC : 1; // Auxiliary Carry flag
    Byte P : 1; // Parity flag
    Byte C : 1; // Carry flag

    /** To Be Implemented **/
    /* Signals */
    Word ResetIn : 1; // RESET CPU signal
    Word ResetOut : 1; // RESET acknowledge - Output
    Word HOLD : 1; // HOLD request 
    Word HLDA : 1; // HOLD acknowledge - Output

    /* Interrupts */
    Word INTR : 1; // Interrupt request - Least priority
    Word INTA : 1; // Interrupt acknowledge - Output
    Word RST5_5 : 1; // 002C
    Word RST6_5 : 1; // 0034
    Word RST7_5 : 1; // 003C
    Word TRAP : 1; // 	0024
    /** To Be Implemented **/

    /* Opcodes */
    static constexpr Byte INS_LDA_IMM = 0x3A; // Load Accumulator Immediate

    public:
    /* Reset routine */
    void Reset( Memory& memory ) {
        PC = 0x0000; // No reset vector for 8085
        IR = 0x00; // Default reset routine
        SP = 0x0100; // Starting stack from random memory location
        
        S = Z = AC = P = C = 0; // Resetting all flags 
        A = 0; // Resetting Accumulator

        memory.Initialize(); // Initializing all memory bits to 0
    }

    /* Fetch one byte from current location of Program Counter */
    Byte FetchByte( u32& cycles, Memory& memory ) {
        Byte Data = memory[PC];
        PC++;
        cycles--;
        return Data;
    }

    /* Fetch -> Decode -> Execute procedure */
    void Execute( u32 cycles, Memory& memory ) {
        while (cycles > 0) {
            IR = FetchByte( cycles, memory );
            switch( IR ) {
                case INS_LDA_IMM:
                {
                    Byte Value = FetchByte( cycles, memory ); // Fetch value to load into Accumulator
                    A = Value; 
                    Z = (A == 0); // Set Zero flag if A == 0
                    S = (A & 0b10000000) > 0; // Set Sign flag if 8th bit of A is 1 i.e. A is negative
                } break;
                default:
                    throw InvalidOpcode();
            }

        }
        

    }

};