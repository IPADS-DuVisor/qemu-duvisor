#include <rvh_test.h>
#include <page_tables.h> 

DECLARE_TEST(u_csr_tests){
    TEST_START();
    uint64_t val;

    /* ------ UTVEC  */
    printf("\n >>> UTVEC\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t utvec;
    // access from M-mode 
    val = 0;
    CSRW(CSR_UTVEC, val);
    utvec = CSRR(CSR_UTVEC);
    TEST_ASSERT("m gets right values", 
            excpt.triggered == false && utvec == val);

    // access from HS mode
	CSRS(medeleg, 1 << CAUSE_ILI);
    goto_priv(PRIV_HS);
    val = 1;
    CSRW(CSR_UTVEC, val);
    utvec = CSRR(CSR_UTVEC);
    TEST_ASSERT("hs gets right values", 
            excpt.triggered == false && utvec == val);

    // access from HU mode
	CSRS(CSR_HSTATUS, HSTATUS_HU);
    goto_priv(PRIV_HU);
    val = 2;
    CSRW(CSR_UTVEC, val);
    utvec = CSRR(CSR_UTVEC);
    TEST_ASSERT("hu gets right values", 
            excpt.triggered == false && utvec == 1);

    /* ------ UEPC  */
    printf("\n >>> UEPC\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t uepc;
    // access from M-mode 
    val = 0x8048000;
    CSRW(CSR_UEPC, val);
    uepc = CSRR(CSR_UEPC);
    TEST_ASSERT("m gets right values", 
            excpt.triggered == false && uepc == val);

    // access from HS mode
	CSRS(medeleg, 1 << CAUSE_ILI);
    goto_priv(PRIV_HS);
    val = 0x100000;
    CSRW(CSR_UEPC, val);
    uepc = CSRR(CSR_UEPC);
    TEST_ASSERT("hs gets right values", 
            excpt.triggered == false && uepc == val);

    // access from HU mode
	CSRS(CSR_HSTATUS, HSTATUS_HU);
    goto_priv(PRIV_HU);
    val = 0xffff1234000;
    CSRW(CSR_UEPC, val);
    uepc = CSRR(CSR_UEPC);
    TEST_ASSERT("hu gets right values", 
            excpt.triggered == false && uepc == val);

    /* ------ USCRATCH  */
    printf("\n >>> USCRATCH\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t uscratch;
    // access from M-mode 
    val = 0x5678000;
    CSRW(CSR_USCRATCH, val);
    uscratch = CSRR(CSR_USCRATCH);
    TEST_ASSERT("m gets right values", 
            excpt.triggered == false && uscratch == val);

    // access from HS mode
	CSRS(medeleg, 1 << CAUSE_ILI);
    goto_priv(PRIV_HS);
    val = 0xffff5678000;
    CSRW(CSR_USCRATCH, val);
    uscratch = CSRR(CSR_USCRATCH);
    TEST_ASSERT("hs gets right values", 
            excpt.triggered == false && uscratch == val);

    // access from HU mode
	CSRS(CSR_HSTATUS, HSTATUS_HU);
    goto_priv(PRIV_HU);
    val = 0x7f09000;
    CSRW(CSR_USCRATCH, val);
    uscratch = CSRR(CSR_USCRATCH);
    TEST_ASSERT("hu gets right values", 
            excpt.triggered == false && uscratch == val);

    /* ------ UCAUSE  */
    printf("\n >>> UCAUSE\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t ucause;
    // access from M-mode 
    val = 21;
    CSRW(CSR_UCAUSE, val);
    ucause = CSRR(CSR_UCAUSE);
    TEST_ASSERT("m gets right values", 
            excpt.triggered == false && ucause == val);

    // access from HS mode
	CSRS(medeleg, 1 << CAUSE_ILI);
    goto_priv(PRIV_HS);
    val = 22;
    CSRW(CSR_UCAUSE, val);
    ucause = CSRR(CSR_UCAUSE);
    TEST_ASSERT("hs gets right values", 
            excpt.triggered == false && ucause == val);

    // access from HU mode
	CSRS(CSR_HSTATUS, HSTATUS_HU);
    goto_priv(PRIV_HU);
    val = 23;
    CSRW(CSR_UCAUSE, val);
    ucause = CSRR(CSR_UCAUSE);
    TEST_ASSERT("hu gets right values", 
            excpt.triggered == false && ucause == val);
    
    /* ------ UTVAL  */
    printf("\n >>> UTVAL\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t utval;
    // access from M-mode 
    val = 0x100000;
    CSRW(CSR_UTVAL, val);
    utval = CSRR(CSR_UTVAL);
    TEST_ASSERT("m gets right values", 
            excpt.triggered == false && utval == val);

    // access from HS mode
	CSRS(medeleg, 1 << CAUSE_ILI);
    goto_priv(PRIV_HS);
    val = 0x200000;
    CSRW(CSR_UTVAL, val);
    utval = CSRR(CSR_UTVAL);
    TEST_ASSERT("hs gets right values", 
            excpt.triggered == false && utval == val);

    // access from HU mode
	CSRS(CSR_HSTATUS, HSTATUS_HU);
    goto_priv(PRIV_HU);
    val = 0x300000;
    CSRW(CSR_UTVAL, val);
    utval = CSRR(CSR_UTVAL);
    TEST_ASSERT("hu gets right values", 
            excpt.triggered == false && utval == val);


    TEST_END();
}
