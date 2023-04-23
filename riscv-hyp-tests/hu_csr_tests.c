#include <rvh_test.h>
#include <page_tables.h> 

DECLARE_TEST(hu_csr_tests){
    TEST_START();
    uint64_t val;

    /* ------ HUSTATUS     */
    printf("\n >>> HUSTATUS\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t hustatus;
    // access from M-mode 
    val = HSTATUS_VSXL_64 | (1 << 20);
    CSRW(CSR_HUSTATUS, val);
    hustatus = CSRR(CSR_HUSTATUS);
    TEST_ASSERT("m gets right values", 
            excpt.triggered == false && hustatus == val);

    // access from HS mode
    CSRS(medeleg, 1 << CAUSE_ILI);
    goto_priv(PRIV_HS);
    val = HSTATUS_VSXL_64 | (1 << 21);
    CSRW(CSR_HUSTATUS, val);
    hustatus = CSRR(CSR_HUSTATUS);
    TEST_ASSERT("hs gets right values", 
            excpt.triggered == false && hustatus == val);

    // access from HU mode
    CSRS(CSR_HSTATUS, HSTATUS_HU);
    goto_priv(PRIV_HU);
    val = HSTATUS_VSXL_64 | (1 << 22);
    CSRW(CSR_HUSTATUS, val);
    hustatus = CSRR(CSR_HUSTATUS);
    TEST_ASSERT("hu gets right values", 
            excpt.triggered == false && hustatus == val);
   
    /* ------ HUIE         */
    printf("\n >>> HUIE\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t huie;
    // access from M-mode 
    val = 1 << 8;
    CSRW(CSR_HUIE, val);
    huie = CSRR(CSR_HUIE);
    TEST_ASSERT("m gets right values", 
            excpt.triggered == false && huie == val);

    // access from HS mode
    CSRS(medeleg, 1 << CAUSE_ILI);
    goto_priv(PRIV_HS);
    val = 1 << 4;
    CSRW(CSR_HUIE, val);
    huie = CSRR(CSR_HUIE);
    TEST_ASSERT("hs gets right values", 
            excpt.triggered == false && huie == val);

    // access from HU mode
    CSRS(CSR_HSTATUS, HSTATUS_HU);
    goto_priv(PRIV_HU);
    val = 1 << 0;
    CSRW(CSR_HUIE, val);
    huie = CSRR(CSR_HUIE);
    TEST_ASSERT("hu gets right values", 
            excpt.triggered == false && huie == val);

    /* ------ HUCOUNTEREN  */
    printf("\n >>> HUCOUNTEREN\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t hucounteren;
    // access from M-mode 
    val = 1 << 0;
    CSRW(CSR_HUCOUNTEREN, val);
    hucounteren = CSRR(CSR_HUCOUNTEREN);
    TEST_ASSERT("m gets right values", 
            excpt.triggered == false && hucounteren == val);

    // access from HS mode
    CSRS(medeleg, 1 << CAUSE_ILI);
    goto_priv(PRIV_HS);
    val = 1 << 1;
    CSRW(CSR_HUCOUNTEREN, val);
    hucounteren = CSRR(CSR_HUCOUNTEREN);
    TEST_ASSERT("hs gets right values", 
            excpt.triggered == false && hucounteren == val);

    // access from HU mode
    CSRS(CSR_HSTATUS, HSTATUS_HU);
    goto_priv(PRIV_HU);
    val = 1 << 2;
    CSRW(CSR_HUCOUNTEREN, val);
    hucounteren = CSRR(CSR_HUCOUNTEREN);
    TEST_ASSERT("hu gets right values", 
            excpt.triggered == false && hucounteren == val);

    /* ------ HUTVAL       */
    printf("\n >>> HUTVAL\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t hutval;
    // access from M-mode 
    val = 1 << 0;
    CSRW(CSR_HUTVAL, val);
    hutval = CSRR(CSR_HUTVAL);
    TEST_ASSERT("m gets right values", 
            excpt.triggered == false && hutval == val);

    // access from HS mode
    CSRS(medeleg, 1 << CAUSE_ILI);
    goto_priv(PRIV_HS);
    val = 1 << 1;
    CSRW(CSR_HUTVAL, val);
    hutval = CSRR(CSR_HUTVAL);
    TEST_ASSERT("hs gets right values", 
            excpt.triggered == false && hutval == val);

    // access from HU mode
    CSRS(CSR_HSTATUS, HSTATUS_HU);
    goto_priv(PRIV_HU);
    val = 1 << 2;
    CSRW(CSR_HUTVAL, val);
    hutval = CSRR(CSR_HUTVAL);
    TEST_ASSERT("hu gets right values", 
            excpt.triggered == false && hutval == val);

    /* ------ HUVIP        */
    uint64_t huvip;
    /* ------ HUIP         */
    uint64_t huip;
    
    /* ------ HUTINST      */
    printf("\n >>> HUTINST\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t hutinst;
    // access from M-mode 
    val = 1 << 0;
    CSRW(CSR_HUTINST, val);
    hutinst = CSRR(CSR_HUTINST);
    TEST_ASSERT("m gets right values", 
            excpt.triggered == false && hutinst == 0);

    // access from HS mode
    CSRS(medeleg, 1 << CAUSE_ILI);
    goto_priv(PRIV_HS);
    val = 1 << 1;
    CSRW(CSR_HUTINST, val);
    hutinst = CSRR(CSR_HUTINST);
    TEST_ASSERT("hs gets right values", 
            excpt.triggered == false && hutinst == 0);

    // access from HU mode
    CSRS(CSR_HSTATUS, HSTATUS_HU);
    goto_priv(PRIV_HU);
    val = 1 << 2;
    CSRW(CSR_HUTINST, val);
    hutinst = CSRR(CSR_HUTINST);
    TEST_ASSERT("hu gets right values", 
            excpt.triggered == false && hutinst == 0);

    /* ------ HUGATP       */
    printf("\n >>> HUGATP\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t hugatp;
    // access from M-mode 
    val = 1 << 0;
    CSRW(CSR_HUGATP, val);
    hugatp = CSRR(CSR_HUGATP);
    TEST_ASSERT("m gets right values", 
            excpt.triggered == false && hugatp == val);

    // access from HS mode
    CSRS(medeleg, 1 << CAUSE_ILI);
    goto_priv(PRIV_HS);
    val = 1 << 1;
    CSRW(CSR_HUGATP, val);
    hugatp = CSRR(CSR_HUGATP);
    TEST_ASSERT("hs gets right values", 
            excpt.triggered == false && hugatp == val);

    // access from HU mode
    CSRS(CSR_HSTATUS, HSTATUS_HU);
    goto_priv(PRIV_HU);
    val = 1 << 2;
    CSRW(CSR_HUGATP, val);
    hugatp = CSRR(CSR_HUGATP);
    TEST_ASSERT("hu gets right values", 
            excpt.triggered == false && hugatp == val);

    /* ------ HUTIMEDELTA  */
    printf("\n >>> HUTIMEDELTA\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t hutimedelta;
    // access from M-mode 
    val = 1 << 0;
    CSRW(CSR_HUTIMEDELTA, val);
    hutimedelta = CSRR(CSR_HUTIMEDELTA);
    TEST_ASSERT("m gets right values", 
            excpt.triggered == false && hutimedelta == val);

    // access from HS mode
    CSRS(medeleg, 1 << CAUSE_ILI);
    goto_priv(PRIV_HS);
    val = 1 << 1;
    CSRW(CSR_HUTIMEDELTA, val);
    hutimedelta = CSRR(CSR_HUTIMEDELTA);
    TEST_ASSERT("hs gets right values", 
            excpt.triggered == false && hutimedelta == val);

    // access from HU mode
    CSRS(CSR_HSTATUS, HSTATUS_HU);
    goto_priv(PRIV_HU);
    val = 1 << 2;
    CSRW(CSR_HUTIMEDELTA, val);
    hutimedelta = CSRR(CSR_HUTIMEDELTA);
    TEST_ASSERT("hu gets right values", 
            excpt.triggered == false && hutimedelta == val);
    
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
