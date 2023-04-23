#include <rvh_test.h>
#include <page_tables.h> 

DECLARE_TEST(hufence_tests){

    TEST_START();
    uintptr_t vaddr;
    uint64_t val;
    bool cond;
    write64(phys_page_base(SWITCH1), 0x111111111);
    write64(phys_page_base(SWITCH2), 0x222222222);

    goto_priv(PRIV_HS);
    goto_priv(PRIV_HU);
    set_prev_priv(PRIV_VS);
    hupt_init();
    vspt_init();
    vaddr = vs_page_base(SWITCH1);

    val = hulvd(vaddr);
    cond = true;
    hupt_switch();
    cond &= hulvd(vaddr) == val;
    hufence_vvma();
    cond &= hulvd(vaddr) != val;
    hupt_switch();
    cond &= hulvd(vaddr) != val;
    hufence_gvma();
    cond &= hulvd(vaddr) == val;
    TEST_ASSERT("hufences correctly invalidate guest tlb entries", cond);
    
    TEST_END();
}
