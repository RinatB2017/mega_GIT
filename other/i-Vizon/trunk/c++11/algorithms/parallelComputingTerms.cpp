//Date: 05/04/2014

/**
 *  Sequential Computing consists of processor + memory + datapath
 */

/// @TIPS   : Pipeline : InstructionFetch - IDecode - OperandF - Execute - WriteBack - NoAction
/**
 *  Superscalar Execution:
 *          Processor having more than one pipeline, capable of issuing
 *          more than one instruction at a time
 *  Eg:
 *      load R1, @1000
 *      load R2, @1008
 *      add R1, @1004
 *      add R2, @100c
 *      add R1, R2
 *      store R1, @2000
 *
 *      T0  T1  T2  T3  T4  T5  T6  T7  T8
 *      IF  ID  OF
 *      IF  ID  OF
 *          IF  ID  OF  E
 *          IF  ID  OF  E
 *              IF  ID  NA  E
 *                  IF  ID  NA  WB

/// @TIPS : Allways parallize optimized serial code
