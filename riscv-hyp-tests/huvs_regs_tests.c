#include <rvh_test.h>
#include <page_tables.h> 

DECLARE_TEST(huvs_regs_tests){
    TEST_START();
    uint64_t huvsstatus;
    uint64_t huvsie;
    uint64_t huvstvec;
    uint64_t huvsscratch;
    uint64_t huvsepc;
    uint64_t huvscause;
    uint64_t huvstval;
    uint64_t huvsip;
    uint64_t huvsatp;

    // access from M-mode 

    CSRW(CSR_HUVSSTATUS, 0x10);
    CSRW(CSR_HUVSIE, 0x2);
    CSRW(CSR_HUVSTVEC, 0x10);
    CSRW(CSR_HUVSSCRATCH, 0x10);
    CSRW(CSR_HUVSEPC, 0x10);
    CSRW(CSR_HUVSCAUSE, 0x10);
    CSRW(CSR_HUVSTVAL, 0x10);
    CSRW(CSR_HUVSATP, 0x10);
    TEST_SETUP_EXCEPT();
    huvsstatus = CSRR(CSR_HUVSSTATUS);
    huvsie = CSRR(CSR_HUVSIE);
    huvstvec = CSRR(CSR_HUVSTVEC);
    huvsscratch = CSRR(CSR_HUVSSCRATCH);
    huvsepc = CSRR(CSR_HUVSEPC);
    huvscause = CSRR(CSR_HUVSCAUSE);
    huvstval = CSRR(CSR_HUVSTVAL);
    huvsatp = CSRR(CSR_HUVSATP);
    VERBOSE("vs get sstatus 0x%lx, 0x%lx, 0x%lx, 0x%lx, 0x%lx, 0x%lx, 0x%lx, 0x%lx ", 
        huvsstatus,  
        huvsie,  
        huvstvec,  
        huvsscratch,  
        huvsepc,  
        huvscause,  
        huvstval,  
        huvsatp
    );
    TEST_ASSERT(
        "m gets right values", 
        excpt.triggered == false &&
        huvsstatus == 0x10 &&
        huvstvec == 0x10 &&
        huvsscratch == 0x10 &&
        huvsepc == 0x10 &&
        huvscause == 0x10 &&
        huvstval == 0x10 &&
        huvsatp == 0x10 
    );

    // access from HS mode
    goto_priv(PRIV_HS);
    CSRW(CSR_HUVSSTATUS, 0x20);
    CSRW(CSR_HUVSTVEC, 0x20);
    CSRW(CSR_HUVSSCRATCH, 0x20);
    CSRW(CSR_HUVSEPC, 0x20);
    CSRW(CSR_HUVSCAUSE, 0x20);
    CSRW(CSR_HUVSTVAL, 0x20);
    CSRW(CSR_HUVSATP, 0x20);
    TEST_SETUP_EXCEPT();
    huvsstatus = CSRR(CSR_HUVSSTATUS);
    huvstvec = CSRR(CSR_HUVSTVEC);
    huvsscratch = CSRR(CSR_HUVSSCRATCH);
    huvsepc = CSRR(CSR_HUVSEPC);
    huvscause = CSRR(CSR_HUVSCAUSE);
    huvstval = CSRR(CSR_HUVSTVAL);
    huvsatp = CSRR(CSR_HUVSATP);
    TEST_ASSERT(
        "hs gets right values", 
        excpt.triggered == false &&
        huvsstatus == 0x20 &&
        huvstvec == 0x20 &&
        huvsscratch == 0x20 &&
        huvsepc == 0x20 &&
        huvscause == 0x20 &&
        huvstval == 0x20 &&
        huvsatp == 0x20 
    );

    // access from HU mode
    goto_priv(PRIV_HU);
    CSRW(CSR_HUVSSTATUS, 0x30);
    CSRW(CSR_HUVSTVEC, 0x30);
    CSRW(CSR_HUVSSCRATCH, 0x30);
    CSRW(CSR_HUVSEPC, 0x30);
    CSRW(CSR_HUVSCAUSE, 0x30);
    CSRW(CSR_HUVSTVAL, 0x30);
    CSRW(CSR_HUVSATP, 0x30);
    TEST_SETUP_EXCEPT();
    huvsstatus = CSRR(CSR_HUVSSTATUS);
    huvstvec = CSRR(CSR_HUVSTVEC);
    huvsscratch = CSRR(CSR_HUVSSCRATCH);
    huvsepc = CSRR(CSR_HUVSEPC);
    huvscause = CSRR(CSR_HUVSCAUSE);
    huvstval = CSRR(CSR_HUVSTVAL);
    huvsatp = CSRR(CSR_HUVSATP);
    TEST_ASSERT(
        "hu gets right values", 
        excpt.triggered == false &&
        huvsstatus == 0x30 &&
        huvstvec == 0x30 &&
        huvsscratch == 0x30 &&
        huvsepc == 0x30 &&
        huvscause == 0x30 &&
        huvstval == 0x30 &&
        huvsatp == 0x30 
    );

    // access from VS mode
    goto_priv(PRIV_HS);
    goto_priv(PRIV_VS);
    excpt.triggered = false;

    huvsstatus = CSRR(sstatus);
    huvstvec = CSRR(stvec);
    huvsscratch = CSRR(sscratch);
    huvsepc = CSRR(sepc);
    huvscause = CSRR(scause);
    huvstval = CSRR(stval);
    huvsatp = CSRR(satp);
    VERBOSE("vs get sstatus 0x%lx", huvsstatus);
    TEST_ASSERT("vs get gets right values",         
        excpt.triggered == false &&
        huvsstatus == 0x30 &&
        huvstvec == 0x30 &&
        huvsscratch == 0x30 &&
        huvsepc == 0x30 &&
        huvscause == 0x30 &&
        huvstval == 0x30 &&
        huvsatp == 0x30 
    );
    TEST_END();


}
