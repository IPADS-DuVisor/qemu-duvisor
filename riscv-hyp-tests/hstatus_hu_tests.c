#include <rvh_test.h>
#include <page_tables.h> 

DECLARE_TEST(hstatus_hu_tests){
    TEST_START();
    uint64_t val = ~0;

    /* ################ CSR ############# */

    /* ------ HUSTATUS     */
    printf("\n >>> HUSTATUS\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t hustatus;
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    hustatus = CSRR(CSR_HUSTATUS);
    TEST_ASSERT("hustatus is readable from HS-mode", 
            excpt.triggered == false);

    // access from HU mode
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    val = CSRR(CSR_HUSTATUS);
    TEST_ASSERT("HU-mode triggers illegal instruciton", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);
  
    
    /* ------ HUIE         */
    printf("\n >>> HUIE\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t huie;
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    huie = CSRR(CSR_HUIE);
    TEST_ASSERT("huie is readable from HS-mode", 
            excpt.triggered == false);

    // access from HU mode
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    val = CSRR(CSR_HUIE);
    TEST_ASSERT("HU-mode triggers illegal instruciton", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);

    /* ------ HUCOUNTEREN  */
    printf("\n >>> HUCOUNTEREN\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t hucounteren;
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    hucounteren = CSRR(CSR_HUCOUNTEREN);
    TEST_ASSERT("hucounteren is readable from HS-mode", 
            excpt.triggered == false);

    // access from HU mode
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    val = CSRR(CSR_HUCOUNTEREN);
    TEST_ASSERT("HU-mode triggers illegal instruciton", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);

    /* ------ HUTVAL       */
    printf("\n >>> HUTVAL\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t hutval;
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    hutval = CSRR(CSR_HUTVAL);
    TEST_ASSERT("hutval is readable from HS-mode", 
            excpt.triggered == false);

    // access from HU mode
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    val = CSRR(CSR_HUTVAL);
    TEST_ASSERT("HU-mode triggers illegal instruciton", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);

    /* ------ HUVIP        */
    uint64_t huvip;
    /* ------ HUIP         */
    uint64_t huip;
    
    /* ------ HUTINST      */
    printf("\n >>> HUTINST\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t hutinst;
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    hutinst = CSRR(CSR_HUTINST);
    TEST_ASSERT("hutinst is readable from HS-mode", 
            excpt.triggered == false);

    // access from HU mode
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    val = CSRR(CSR_HUTINST);
    TEST_ASSERT("HU-mode triggers illegal instruciton", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);

    /* ------ HUGATP       */
    printf("\n >>> HUGATP\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t hugatp;
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    hugatp = CSRR(CSR_HUGATP);
    TEST_ASSERT("hugatp is readable from HS-mode", 
            excpt.triggered == false);

    // access from HU mode
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    val = CSRR(CSR_HUGATP);
    TEST_ASSERT("HU-mode triggers illegal instruciton", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);

    /* ------ HUTIMEDELTA  */
    printf("\n >>> HUTIMEDELTA\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t hutimedelta;
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    hutimedelta = CSRR(CSR_HUTIMEDELTA);
    TEST_ASSERT("hutimedelta is readable from HS-mode", 
            excpt.triggered == false);

    // access from HU mode
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    val = CSRR(CSR_HUTIMEDELTA);
    TEST_ASSERT("HU-mode triggers illegal instruciton", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);
    
    /* ------ UCAUSE  */
    printf("\n >>> UCAUSE\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t ucause;
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    ucause = CSRR(CSR_UCAUSE);
    TEST_ASSERT("ucause is readable from HS-mode", 
            excpt.triggered == false);

    // access from HU mode
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    val = CSRR(CSR_UCAUSE);
    TEST_ASSERT("HU-mode triggers illegal instruciton", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);
    
    /* ------ UTVAL  */
    printf("\n >>> UTVAL\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t utval;
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    utval = CSRR(CSR_UTVAL);
    TEST_ASSERT("utval is readable from HS-mode", 
            excpt.triggered == false);

    // access from HU mode
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    val = CSRR(CSR_UTVAL);
    TEST_ASSERT("HU-mode triggers illegal instruciton", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);
    
    /* ------ UEPC  */
    printf("\n >>> UEPC\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t uepc;
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    uepc = CSRR(CSR_UEPC);
    TEST_ASSERT("uepc is readable from HS-mode", 
            excpt.triggered == false);

    // access from HU mode
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    val = CSRR(CSR_UEPC);
    TEST_ASSERT("HU-mode triggers illegal instruciton", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);
    
    /* ------ USCRATCH  */
    printf("\n >>> USCRATCH\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t uscratch;
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    uscratch = CSRR(CSR_USCRATCH);
    TEST_ASSERT("uscratch is readable from HS-mode", 
            excpt.triggered == false);

    // access from HU mode
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    val = CSRR(CSR_USCRATCH);
    TEST_ASSERT("HU-mode triggers illegal instruciton", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);
    
    /* ------ UTVEC  */
    printf("\n >>> UTVEC\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    uint64_t utvec;
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    utvec = CSRR(CSR_UTVEC);
    TEST_ASSERT("utvec is readable from HS-mode", 
            excpt.triggered == false);

    // access from HU mode
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    val = CSRR(CSR_UTVEC);
    TEST_ASSERT("HU-mode triggers illegal instruciton", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);



    /* ################ INSTRUCTION ############# */
    
    /* ------ HURET ---------- */
    printf("\n >>> HURET\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    asm volatile("uret");
    TEST_ASSERT("uret is illegal instruciton When HU is not set", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);

    uintptr_t dummy_vaddr = 0xbeefcafe;
    uint64_t dummy;

    /* ------ hulv ---------- */
    printf("\n >>> HULV_D\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    dummy =  hulvd(dummy_vaddr);
    
    TEST_ASSERT("hulvd is illegal instruciton When HU is not set", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);

    /* ------ hulv ---------- */
    printf("\n >>> HULV_W\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    dummy =  hulvw(dummy_vaddr);
    
    TEST_ASSERT("hulvw is illegal instruciton When HU is not set", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);

    /* ------ hulv ---------- */
    printf("\n >>> HULV_WU\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    dummy =  hulvwu(dummy_vaddr);
    
    TEST_ASSERT("hulvhwu is illegal instruciton When HU is not set", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);

    /* ------ hulv ---------- */
    printf("\n >>> HULV_H\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    dummy =  hulvh(dummy_vaddr);
    
    TEST_ASSERT("hulvh is illegal instruciton When HU is not set", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);

    /* ------ hulv ---------- */
    printf("\n >>> HULV_HU\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    dummy =  hulvhu(dummy_vaddr);
    
    TEST_ASSERT("hulvhu is illegal instruciton When HU is not set", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);


    /* ------ hulv ---------- */
    printf("\n >>> HULV_B\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    dummy =  hulvb(dummy_vaddr);
    
    TEST_ASSERT("hulvb is illegal instruciton When HU is not set", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);

    /* ------ hulv ---------- */
    printf("\n >>> HULV_BU\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    dummy =  hulvbu(dummy_vaddr);
    
    TEST_ASSERT("hulvbu is illegal instruciton When HU is not set", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);

    /* ------ hslv ---------- */
    printf("\n >>> HUSV_D\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    husvd(dummy_vaddr, dummy);
    TEST_ASSERT("husvd is illegal instruciton When HU is not set", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);
    
    /* ------ hslv ---------- */
    printf("\n >>> HUSV_W\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    husvw(dummy_vaddr, dummy);
    TEST_ASSERT("husvw is illegal instruciton When HU is not set", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);

    /* ------ hslv ---------- */
    printf("\n >>> HUSV_H\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    husvh(dummy_vaddr, dummy);
    TEST_ASSERT("husvh is illegal instruciton When HU is not set", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);

    /* ------ hslv ---------- */
    printf("\n >>> HUSV_B\n");
    reset_state();
    TEST_SETUP_EXCEPT();
    // clear HSTATUS.HU in M mode
    CSRC(CSR_HSTATUS, HSTATUS_HU);
    INFO("\t\tHSTATUS.HU is cleared");
    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    husvb(dummy_vaddr, dummy);
    TEST_ASSERT("husvb is illegal instruciton When HU is not set", 
            excpt.triggered == true && excpt.cause == CAUSE_ILI);
    TEST_END();
}
