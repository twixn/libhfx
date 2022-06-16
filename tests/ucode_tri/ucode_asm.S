#include "tri.S"

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
    jr ra
    nop

.global output_data
output_data:
    // output vector registers to memory
    sqv $v16, 0, 0, $0
    sqv $v17, 0, 16, $0
    sqv $v18, 0, 32, $0
    sqv $v19, 0, 48, $0
    sqv $v20, 0, 64, $0
    sqv $v21, 0, 80, $0 
    sqv $v14, 0, 96, $0
    sqv $v15, 0, 112, $0
    jr ra
    nop
