#include "mat_vec.S"

.global setup_data
setup_data:
    // Initalize $4 to 0
    li $4, 0 

    // Load test data in vector registers
    lqv $v04, 0, 0, $0
    lqv $v05, 0, 16, $0
    lqv $v06, 0, 32, $0
    lqv $v07, 0, 48, $0
    lqv $v08, 0, 64, $0
    lqv $v09, 0, 80, $0   
    lqv $v10, 0, 96, $0   
    lqv $v11, 0, 112, $0   
    lqv $v12, 0, 128, $0   
    lqv $v13, 0, 144, $0   
    jr ra
    nop

.global output_data
output_data:
    // output vector registers to memory
    ssv $v04, 0, 0, $0
    ssv $v05, 0, 2, $0
    ssv $v04, 2, 4, $0
    ssv $v05, 2, 6, $0
    ssv $v04, 4, 8, $0
    ssv $v05, 4, 10, $0
    ssv $v04, 6, 12, $0
    ssv $v05, 6, 14, $0

    jr ra
    nop
