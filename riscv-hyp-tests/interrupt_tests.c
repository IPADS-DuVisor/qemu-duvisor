#include <rvh_test.h>

DECLARE_TEST(check_xip_regs){

    TEST_START();

    CSRW(mideleg, 0);

    CSRW(mideleg, (uint64_t)-1);
    VERBOSE("setting mideleg and hideleg\n");
    CSRW(CSR_HIDELEG, (uint64_t)-1);
    check_csr_wrrd("vsip", CSR_VSIP, (uint64_t) -1, 0x2);
    check_csr_wrrd("vsie", CSR_VSIE, (uint64_t) -1, 0x222);

    VERBOSE("setting all in mip\n");
    CSRW(mip, (uint64_t)-1);
    check_csr_rd("hip", CSR_HIP, 0x4);
    check_csr_rd("sip", sip, 0x222);
    check_csr_rd("mip", mip, 0x226);
    check_csr_rd("vsip", CSR_VSIP, 0x2);
    goto_priv(PRIV_VS);
    check_csr_rd("sip (vs perspective)", sip, 0x2);
    goto_priv(PRIV_M);

    VERBOSE("clearing all in mip\n");
    CSRW(mip, (uint64_t)0);
    check_csr_rd("hip", CSR_HIP, 0x0);
    check_csr_rd("sip", sip, 0x0);
    check_csr_rd("mip", mip, 0x000);
    check_csr_rd("vsip", CSR_VSIP, 0x0);
    goto_priv(PRIV_VS);
    check_csr_rd("sip (vs perspective)", sip, 0x0);
    goto_priv(PRIV_M);   

    VERBOSE("setting all in hvip\n");
    CSRW(CSR_HVIP, (uint64_t)-1);
    check_csr_rd("hvip", CSR_HVIP, 0x444);
    check_csr_rd("hip", CSR_HIP, 0x444);
    check_csr_rd("sip", sip, 0x0);
    check_csr_rd("mip", mip, 0x444);
    check_csr_rd("vsip", CSR_VSIP, 0x222);
    goto_priv(PRIV_VS);
    check_csr_rd("sip (vs perspective)", sip, 0x222);
    goto_priv(PRIV_M);

    VERBOSE("clearing all in hvip\n");
    CSRW(CSR_HVIP, (uint64_t)0);
    check_csr_rd("hip", CSR_HIP, 0x0);
    check_csr_rd("sip", sip, 0x0);
    check_csr_rd("mip", mip, 0x000);
    check_csr_rd("vsip", CSR_VSIP, 0x0);
    goto_priv(PRIV_VS);
    check_csr_rd("sip (vs perspective)", sip, 0x0);
    goto_priv(PRIV_M);

    TEST_END();
}

DECLARE_TEST(interrupt_hideleg_tests){

    TEST_START();

    /**
     * Test trigerring VSSI without delegating it. 
     * It assumes it is already delegated in miedeleg (it should be hardwired)
     */
    goto_priv(PRIV_M);
    reset_state();
    goto_priv(PRIV_HS);
    DEBUG("HS-mode: clear SIE + set SPIE -> sret will set SIE to SPIE\n");
    CSRC(sstatus, SSTATUS_SIE_BIT);
    CSRS(sstatus, SSTATUS_SPIE_BIT);
    DEBUG("HS-mode: VSSI is pending, but IRQ is disabled in HS-mode\n");
    CSRS(CSR_HIE, 0x4);
    CSRS(CSR_HIP, 0x4);
    TEST_SETUP_EXCEPT();
    DEBUG("HS-mode: sret to VS -> enable IRQ in HS-mode\n");
    goto_priv(PRIV_VS);
    
    TEST_ASSERT("from VS to HS without hideleg", 
        excpt.triggered && excpt.cause == CAUSE_VSSI && excpt.priv == PRIV_HS);

    /**
     * Test trigerring VSSI and delegating it. Should trap to VS with cause SSI.
     * It assumes it is already delegated in miedeleg (it should be hardwired)
     */
    goto_priv(PRIV_M);
    reset_state();
    CSRS(mideleg, 0x4);
    goto_priv(PRIV_HS);
    DEBUG("HS-mode: deleg VSSI to V-mode\n");
    CSRS(CSR_HIDELEG, 0x4);
    goto_priv(PRIV_HU);
    DEBUG("HU-mode: VSSI is pending\n");
    CSRS(CSR_HUVIP, 0x4);
    TEST_SETUP_EXCEPT();
    goto_priv(PRIV_VS);
    DEBUG("VS-mode: enable IRQ in VS-mode\n");
    CSRS(sie, 0x2);
    CSRS(sstatus, 0x2);

    TEST_ASSERT("from VS to VS with hideleg", 
        excpt.triggered && excpt.cause == CAUSE_SSI && excpt.priv == PRIV_VS);

    TEST_END();
}

DECLARE_TEST(interrupt_sideleg_tests){

    TEST_START();

    /**
     * Test trigerring USI without delegating it. 
     * It assumes it is already delegated in miedeleg (it should be hardwired)
     */
    goto_priv(PRIV_M);
    reset_state();
    CSRS(mideleg, 0x1);
    goto_priv(PRIV_HS);
    DEBUG("HS-mode: clear SIE + set SPIE -> sret will set SIE to SPIE\n");
    CSRC(sstatus, SSTATUS_SIE_BIT);
    CSRS(sstatus, SSTATUS_SPIE_BIT);
    DEBUG("HS-mode: USI is pending, but IRQ is disabled in HS-mode\n");
    CSRS(sie, 0x1);
    CSRS(sip, 0x1);
    TEST_SETUP_EXCEPT();
    DEBUG("HS-mode: sret to HU -> enable IRQ in HS-mode\n");
    goto_priv(PRIV_HU);
    
    TEST_ASSERT("from HU to HS without hideleg", 
        excpt.triggered && excpt.cause == CAUSE_USI && excpt.priv == PRIV_HS);

    /**
     * Test trigerring USI and delegating it to HU. Should trap to HU with cause USI.
     */
    goto_priv(PRIV_M);
    reset_state();
    CSRS(mideleg, 0x1);
    goto_priv(PRIV_HS);
    DEBUG("HS-mode: deleg USI to HU-mode\n");
    CSRS(CSR_SIDELEG, 0x1);
    DEBUG("HS-mode: enable IRQ in HU-mode\n");
    CSRS(CSR_HUIP, 0x1);
    CSRS(CSR_HUIE, 0x1);
    CSRS(CSR_HUSTATUS, 0x1);
    TEST_SETUP_EXCEPT();
    DEBUG("HS-mode: sret to VS-mode -> IRQ for HU-mode\n");
    goto_priv(PRIV_VS);

    TEST_ASSERT("from VS to HU with sideleg", 
        excpt.triggered && excpt.cause == CAUSE_USI && excpt.priv == PRIV_HU);
    
    /**
     * Test trigerring USI and delegating it to HU. Should trap to HU with cause USI.
     */
    goto_priv(PRIV_M);
    reset_state();
    CSRS(mideleg, 0x1);
    goto_priv(PRIV_HS);
    DEBUG("HS-mode: deleg USI to HU-mode\n");
    CSRS(CSR_SIDELEG, 0x1);
    DEBUG("HS-mode: enable IRQ in HU-mode\n");
    CSRS(CSR_HUIP, 0x1);
    CSRS(CSR_HUIE, 0x1);
    CSRS(CSR_HUSTATUS, 0x1);
    TEST_SETUP_EXCEPT();
    DEBUG("HS-mode: sret to HU-mode -> IRQ for HU-mode\n");
    goto_priv(PRIV_HU);

    TEST_ASSERT("from HU to HU with sideleg", 
        excpt.triggered && excpt.cause == CAUSE_USI && excpt.priv == PRIV_HU);

    TEST_END();
}

DECLARE_TEST(exception_sedeleg_tests){

    TEST_START();

    /**
     * Test trigerring ECALL exception without delegating it. 
     */

    goto_priv(PRIV_HS);
    TEST_SETUP_EXCEPT();
    goto_priv(PRIV_VS);
    ecall(ECALL_TESTING,0);

    TEST_ASSERT("from VS to HS", 
        excpt.triggered && excpt.cause == CAUSE_ECV && excpt.priv == PRIV_HS);


    /**
     * Test trigerring Store/AMO access fault exception with s delegation. 
     */
 
    goto_priv(PRIV_M);
    CSRS(medeleg, 1<<7);
    goto_priv(PRIV_HS);
    // Store/AMO access fault
    CSRS(CSR_SEDELEG, 1<<7);
    goto_priv(PRIV_HU);
    TEST_SETUP_EXCEPT();
    uint64_t* dummy_ptr = (uint64_t*)0xffffffffffffffff;
    *dummy_ptr = 1;
    DEBUG("cause %d, priv %d\n" ,excpt.cause, excpt.priv);
    TEST_ASSERT("from HU to HU", 
        excpt.triggered && excpt.cause == CAUSE_SAF && excpt.priv == PRIV_HU);

    TEST_END();
}
extern void hshandler_entry();
extern void huhandler_entry();
extern void mhandler_entry();
extern void vshandler_entry();

#define VIRT_CLINT 0x2000000
#define TIMERCMP_BASE (VIRT_CLINT + 0x4000 + TEST_CPU_NUM * 8)
#define VTIMERCMP_BASE (VIRT_CLINT + 0x10000 + TEST_CPU_NUM * 8)
#define VTIMERCTL_BASE (VIRT_CLINT + 0x10800 + TEST_CPU_NUM * 4)

DECLARE_TEST(vtimer_vstip_tests){
    uint32_t* vtimercmp_base_lo = (uint32_t*)(VTIMERCMP_BASE);
    uint32_t* vtimerctl_base = (uint32_t*)VTIMERCTL_BASE;
    TEST_START();
    

    goto_priv(PRIV_M);
    CSRW(mideleg, 1 << 0x6 | 1<<0x5);
    goto_priv(PRIV_HS);
    CSRW(CSR_HIDELEG, 1<<0x6);
    goto_priv(PRIV_HU);

    // delegate VS TIMER to V mode
    CSRW(CSR_HUTIMEDELTA, 0);
    goto_priv(PRIV_VS);
    // disable VS interrupt
    CSRC(sstatus, 0x2);

    *vtimerctl_base = 0x1 | (0x6 << 1);
    *vtimercmp_base_lo = 10000000;
    *vtimerctl_base = 0;
    *vtimerctl_base = 0x1 | (0x6 << 1);
    *vtimercmp_base_lo = 10000000;
    
    while((CSRR(sip) & 0x20) == 0);
    TEST_END();
}

DECLARE_TEST(vtimer_uvtip_tests){
    uint32_t* vtimercmp_base_lo = (uint32_t*)(VTIMERCMP_BASE);
    uint32_t* vtimerctl_base = (uint32_t*)VTIMERCTL_BASE;
    TEST_START();
    

    goto_priv(PRIV_M);
    CSRW(mideleg, 1 << 0x10);
    goto_priv(PRIV_HS);
    CSRW(CSR_SIDELEG, 1 << 0x10);
    goto_priv(PRIV_HU);

    // disable HU interrupt
    CSRC(CSR_HUSTATUS, 0x1);

    *vtimerctl_base = 0x1 | (0x10 << 1);
    *vtimercmp_base_lo = 20000000;

    while((CSRR(CSR_HUIP) & 0x10000) == 0);
    TEST_END();
}

extern int vtimer_vstip_handled_flag;
DECLARE_TEST(vtimer_vstip_handler_tests){
    uint32_t* vtimercmp_base_lo = (uint32_t*)(VTIMERCMP_BASE);
    uint32_t* vtimerctl_base = (uint32_t*)VTIMERCTL_BASE;
    TEST_START();
    

    goto_priv(PRIV_M);
    CSRW(mideleg, 1 << 0x6 );
    goto_priv(PRIV_HS);
    CSRW(CSR_HIDELEG, 1<<0x6);
    CSRW(CSR_HIE, 1<<0x6);
    goto_priv(PRIV_HU);

    // delegate VS TIMER to V mode
    CSRW(CSR_HUTIMEDELTA, 0);

    goto_priv(PRIV_VS);
    // enable VS interrupt
    CSRS(sstatus, 0x2);
    
    TEST_SETUP_EXCEPT();
    *vtimerctl_base = 0x1 | (0x6 << 1);
    *vtimercmp_base_lo = 30000000;

    
    while(vtimer_vstip_handled_flag == 0);
    TEST_END();
}

extern int vtimer_uvtip_handled_flag;
DECLARE_TEST(vtimer_uvtip_handler_tests){
    uint32_t* vtimercmp_base_lo = (uint32_t*)(VTIMERCMP_BASE);
    uint32_t* vtimerctl_base = (uint32_t*)VTIMERCTL_BASE;
    TEST_START();
    

    goto_priv(PRIV_M);
    CSRW(mideleg, 1 << 0x10);
    goto_priv(PRIV_HS);
    CSRW(CSR_SIDELEG, 1 << 0x10);
    goto_priv(PRIV_HU);
    CSRW(CSR_HUIE, 1 << 0x10);

    // enable HU interrupt
    CSRS(CSR_HUSTATUS, 0x1);

    TEST_SETUP_EXCEPT();
    *vtimerctl_base = 0x1 | (0x10 << 1);
    *vtimercmp_base_lo = 40000000;

    while(vtimer_uvtip_handled_flag == 0);
    vtimer_uvtip_handled_flag = 0;
    TEST_END();

}

DECLARE_TEST(vtimer_regs_vstip_tests){
    TEST_START();
    

    goto_priv(PRIV_M);
    CSRW(mideleg, 1 << 0x6 | 1<<0x5);
    goto_priv(PRIV_HS);
    CSRW(CSR_HIDELEG, 1<<0x6);
    goto_priv(PRIV_HU);

    // delegate VS TIMER to V mode
    CSRW(CSR_HUTIMEDELTA, 0);
    goto_priv(PRIV_VS);
    // disable VS interrupt
    CSRC(sstatus, 0x2);

    CSRW(CSR_VTIMECTL, 0x1 | (0x6 << 1));
    CSRW(CSR_VTIMECMP, 5000000);
    
    while((CSRR(sip) & 0x20) == 0);
    TEST_END();
}


DECLARE_TEST(vtimer_regs_uvtip_tests){
    TEST_START();
    
    goto_priv(PRIV_M);
    CSRW(mideleg, 1 << 0x10);
    goto_priv(PRIV_HS);
    CSRW(CSR_SIDELEG, 1 << 0x10);
    goto_priv(PRIV_HU);

    // disable HU interrupt
    CSRC(CSR_HUSTATUS, 0x1);

    CSRW(CSR_VTIMECTL, 0x1 | (0x10 << 1));
    CSRW(CSR_VTIMECMP, 5000000);

    while((CSRR(CSR_HUIP) & 0x10000) == 0);

    TEST_END();
}

extern int vtimer_uvtip_handled_flag;
DECLARE_TEST(vtimer_regs_uvtip_handler_tests){
    uint32_t* vtimercmp_base_lo = (uint32_t*)(VTIMERCMP_BASE);
    uint32_t* vtimerctl_base = (uint32_t*)VTIMERCTL_BASE;
    TEST_START();
    

    goto_priv(PRIV_M);
    CSRW(mideleg, 1 << 0x10);
    goto_priv(PRIV_HS);
    CSRW(CSR_SIDELEG, 1 << 0x10);
    goto_priv(PRIV_HU);
    CSRW(CSR_HUIE, 1 << 0x10);

    // enable HU interrupt
    CSRS(CSR_HUSTATUS, 0x1);

    TEST_SETUP_EXCEPT();
    CSRW(CSR_VTIMECTL, 0x1 | (0x10 << 1));
    CSRW(CSR_VTIMECMP, 5000000);

    while(vtimer_uvtip_handled_flag == 0);
    vtimer_uvtip_handled_flag = 0;
    TEST_END();

}

#define USIP (1<<0)

DECLARE_TEST(vipi_m_test){
    uint64_t mip;
    uint64_t vipi;
    uint64_t vcpuid;
    uint64_t* vipi_ptr = 0x000002015000L;
    uint64_t* vcpuid_ptr = (0x000002015800L + TEST_CPU_NUM * 8);
    TEST_START();
    reset_state();
    goto_priv(PRIV_M);
    TEST_SETUP_EXCEPT();
    /*
     * read write in M tests
     */
    // init ipi
    vipi = *vipi_ptr;
    vcpuid = *vcpuid_ptr;
    printf("M after write: vipi 0x%lx, vcpuid 0x%lx excpt.triggered %d\n", vipi, vcpuid,excpt.triggered);
    TEST_ASSERT(
       "Init value tests", 
        excpt.triggered == false && vipi == 0 && vcpuid == 0
    );

    // vipi0
    *vipi_ptr = 1 << 23;
    *vcpuid_ptr = 12;
    vipi = *vipi_ptr;
    vcpuid = *vcpuid_ptr;
    printf("M after write: vipi 0x%lx, vcpuid 0x%lx\n", vipi, vcpuid);
    TEST_ASSERT(
        "vipi0 set test", 
        excpt.triggered == false &&  vipi == 1<< 23 && vcpuid == 12
    );

    // vipi1 
    *(vipi_ptr + 1) = 1 << 10;
    *vcpuid_ptr = 9;
    vipi = *(vipi_ptr+1);
    vcpuid = *vcpuid_ptr;
    printf("M after write: vipi 0x%lx, vcpuid 0x%lx\n", vipi, vcpuid);
    TEST_ASSERT(
        "vipi1 set test", 
        excpt.triggered == false &&  vipi == 1<<10 && vcpuid == 9
    );

    // vipi2 
    *(vipi_ptr + 2) = 1L << 60;
    *vcpuid_ptr = 78;
    vipi = *(vipi_ptr + 2);
    vcpuid = *vcpuid_ptr;
    printf("M after write: vipi 0x%lx, vcpuid 0x%lx\n", vipi, vcpuid);
    TEST_ASSERT(
        "vipi2 set test", 
        excpt.triggered == false &&  vipi == 1L<<60 && vcpuid == 78
    );

    // vipi3 
    *(vipi_ptr + 3) = 1 << 1;
    *vcpuid_ptr = 99;
    vipi = *(vipi_ptr + 3);
    vcpuid = *vcpuid_ptr;
    printf("M after write: vipi 0x%lx, vcpuid 0x%lx\n", vipi, vcpuid);
    TEST_ASSERT(
        "vipi3 set test", 
        excpt.triggered == false &&  vipi == 1<<1 && vcpuid == 99
    );

    /*
     * usip bit set in M tests
     */
    // vipi0 set
    *vcpuid_ptr = 23;
    for(int i = 0; i < 100; i++);
    mip = CSRR(mip);
    printf("M after write: mip 0x%lx\n", mip);
    TEST_ASSERT(
        "vcpuid & vipi0 set test", 
        excpt.triggered == false &&  (mip & USIP) == USIP
    );

    // vipi0 clear
    *vipi_ptr = 0;
    CSRC(mip, USIP);
    for(int i = 0; i < 100; i++);
    mip = CSRR(mip);
    printf("M after write: mip 0x%lx\n", mip);
    TEST_ASSERT(
        "vipi0 clear test", 
        excpt.triggered == false &&  (mip & USIP) == 0
    );

    // vipi1 set
    *vcpuid_ptr = 64 + 10;
    for(int i = 0; i < 100; i++);
    mip = CSRR(mip);
    printf("M after write: mip 0x%lx\n", mip);
    TEST_ASSERT(
        "vcpuid & vipi1 set test", 
        excpt.triggered == false &&  (mip & USIP) == USIP
    );

    // vipi1 clear
    *(vipi_ptr + 1) = 0;
    CSRC(mip, USIP);
    for(int i = 0; i < 100; i++);
    mip = CSRR(mip);
    printf("M after write: mip 0x%lx\n", mip);
    TEST_ASSERT(
        "vipi1 clear test", 
        excpt.triggered == false &&  (mip & USIP) == 0
    );

    // vipi2 set
    *vcpuid_ptr = 64*2 + 60;
    for(int i = 0; i < 100; i++);
    mip = CSRR(mip);
    printf("M after write: mip 0x%lx\n", mip);
    TEST_ASSERT(
        "vcpuid & vipi2 set test", 
        excpt.triggered == false &&  (mip & USIP) == USIP
    );

    // vipi2 clear
    *(vipi_ptr + 2) = 0;
    CSRC(mip, USIP);
    for(int i = 0; i < 100; i++);
    mip = CSRR(mip);
    printf("M after write: mip 0x%lx\n", mip);
    TEST_ASSERT(
        "vipi2 clear test", 
        excpt.triggered == false &&  (mip & USIP) == 0
    );

    // vipi3 set
    *vcpuid_ptr = 64*3 + 1;
    for(int i = 0; i < 100; i++);
    mip = CSRR(mip);
    printf("M after write: mip 0x%lx\n", mip);
    TEST_ASSERT(
        "vcpuid & vipi3 set test", 
        excpt.triggered == false &&  (mip & USIP) == USIP
    );

    // vipi3 clear
    *(vipi_ptr + 3) = 0;
    CSRC(mip, USIP);
    for(int i = 0; i < 100; i++);
    mip = CSRR(mip);
    printf("M after write: mip 0x%lx\n", mip);
    TEST_ASSERT(
       "vipi3 clear test", 
        excpt.triggered == false &&  (mip & USIP) == 0
    );
    TEST_END();
}

uint64_t rdvcpuid(){
    return CSRR(CSR_VCPUID);
}

void wrvcpuid(uint64_t vcpuid){
    CSRW(CSR_VCPUID, vcpuid);
}

uint64_t rdvipi0(){
    return CSRR(CSR_VIPI0);
}
uint64_t rdvipi1(){
    return CSRR(CSR_VIPI1);
}
uint64_t rdvipi2(){
    return CSRR(CSR_VIPI2);
}
uint64_t rdvipi3(){
    return CSRR(CSR_VIPI3);
}

void stvipi0(uint64_t vipi){
    CSRS(CSR_VIPI0, vipi);
}
void stvipi1(uint64_t vipi){
    CSRS(CSR_VIPI1, vipi);
}
void stvipi2(uint64_t vipi){
    CSRS(CSR_VIPI2, vipi);
}
void stvipi3(uint64_t vipi){
    CSRS(CSR_VIPI3, vipi);
}

void clvipi0(uint64_t vipi){
    CSRC(CSR_VIPI0, ~vipi);
}
void clvipi1(uint64_t vipi){
    CSRC(CSR_VIPI1, ~vipi);
}
void clvipi2(uint64_t vipi){
    CSRC(CSR_VIPI2, ~vipi);
}
void clvipi3(uint64_t vipi){
    CSRC(CSR_VIPI3, ~vipi);
}


DECLARE_TEST(vipi_hs_test) {
    uint64_t sip, sip_before_clear;
    uint64_t vipi;
    uint64_t vcpuid;

    TEST_START();
    reset_state();
    goto_priv(PRIV_M);
    CSRS(mideleg, USIP);
    goto_priv(PRIV_HS);
    TEST_SETUP_EXCEPT();
    /*
     * read write in HS tests
     */
    // init ipi
    vipi = rdvipi0();
    vcpuid = rdvcpuid();
    printf("HS after write: vipi 0x%lx, vcpuid 0x%lx\n", vipi, vcpuid);
    TEST_ASSERT(
       "Init value tests", 
        excpt.triggered == false && vipi == 0 && vcpuid == 0
    );

    // vipi0
    stvipi0(1 << 22);
    wrvcpuid(11);
    vipi = rdvipi0();
    vcpuid = rdvcpuid();
    printf("HS after write: vipi 0x%lx, vcpuid 0x%lx\n", vipi, vcpuid);
    TEST_ASSERT(
        "vipi0 set test", 
        excpt.triggered == false &&  vipi == 1<<22 && vcpuid == 11
    );

    // vipi1 
    stvipi1(1<<9);
    wrvcpuid(8);
    vipi = rdvipi1();
    vcpuid = rdvcpuid();
    printf("HS after write: vipi 0x%lx, vcpuid 0x%lx\n", vipi, vcpuid);
    TEST_ASSERT(
        "vipi1 set test", 
        excpt.triggered == false &&  vipi == 1<<9 && vcpuid == 8
    );

    // vipi2 
    stvipi2(1L<<59);
    wrvcpuid(77);
    vipi = rdvipi2();
    vcpuid = rdvcpuid();
    printf("HS after write: vipi 0x%lx, vcpuid 0x%lx\n", vipi, vcpuid);
    TEST_ASSERT(
        "vipi2 set test", 
        excpt.triggered == false &&  vipi == 1L<<59 && vcpuid == 77
    );

    // vipi3 
    stvipi3(1<<0);
    wrvcpuid(98);
    vipi = rdvipi3();
    vcpuid = rdvcpuid();
    printf("HS after write: vipi 0x%lx, vcpuid 0x%lx\n", vipi, vcpuid);
    TEST_ASSERT(
        "vipi3 set test", 
        excpt.triggered == false &&  vipi == 1<<0 && vcpuid == 98
    );

    /*
     * usip bit set in HS tests
     */
    // vipi0 set
    wrvcpuid(22);
    for(int i = 0; i < 100; i++);
    sip = CSRR(sip);
    printf("HS after write: sip 0x%lx\n", sip);
    TEST_ASSERT(
        "vcpuid & vipi0 set test", 
        excpt.triggered == false &&  (sip & USIP) == USIP
    );

    // vipi0 clear
    clvipi0( ~(1L << 22) );
    CSRC(sip, USIP);
    for(int i = 0; i < 100; i++);
    sip = CSRR(sip);
    printf("HS after write: sip 0x%lx\n", sip);
    TEST_ASSERT(
       "vipi0 clear test", 
        excpt.triggered == false &&  (sip & USIP) == 0
    );

    // vipi1 set
    wrvcpuid(64 + 9);
    for(int i = 0; i < 100; i++);
    sip = CSRR(sip);
    printf("HS after write: sip 0x%lx\n", sip);
    TEST_ASSERT(
        "vcpuid & vipi1 set test", 
        excpt.triggered == false &&  (sip & USIP) == USIP
    );
    // vipi1 clear
    clvipi1( ~(1L << 9) );
    CSRC(sip, USIP);
    for(int i = 0; i < 100; i++);
    sip = CSRR(sip);
    printf("HS after write: sip 0x%lx\n", sip);
    TEST_ASSERT(
       "vipi1 clear test", 
        excpt.triggered == false &&  (sip & USIP) == 0
    );

    // vipi2 set
    wrvcpuid(64 * 2 + 59);
    for(int i = 0; i < 100; i++);
    sip = CSRR(sip);
    printf("HS after write: sip 0x%lx\n", sip);
    TEST_ASSERT(
        "vcpuid & vipi2 set test", 
        excpt.triggered == false &&  (sip & USIP) == USIP
    );
    // vipi2 clear
    clvipi2( ~(1L << 59) );
    CSRC(sip, USIP);
    for(int i = 0; i < 100; i++);
    sip = CSRR(sip);
    printf("HS after write: sip 0x%lx\n", sip);
    TEST_ASSERT(
       "vipi2 clear test", 
        excpt.triggered == false &&  (sip & USIP) == 0
    );

    // vipi3 set
    wrvcpuid(64 * 3 + 0);
    for(int i = 0; i < 100; i++);
    sip = CSRR(sip);
    printf("HS after write: sip 0x%lx\n", sip);
    TEST_ASSERT(
        "vcpuid & vipi3 set test", 
        excpt.triggered == false &&  (sip & USIP) == USIP
    );
    // vipi3 clear
    clvipi3( ~(1L << 0) );
    CSRC(sip, USIP);
    for(int i = 0; i < 100; i++);
    sip = CSRR(sip);
    printf("HS after write: sip 0x%lx\n", sip);
    TEST_ASSERT(
       "vipi3 clear test", 
        excpt.triggered == false &&  (sip & USIP) == 0
    );
    TEST_END();
}


DECLARE_TEST(vipi_hu_test)
{
    uint64_t huip;
    uint64_t vipi;
    uint64_t vcpuid;

    TEST_START();
    reset_state();
    goto_priv(PRIV_M);
    CSRS(mideleg, USIP);
    goto_priv(PRIV_HS);
    CSRS(CSR_SIDELEG, USIP);
    goto_priv(PRIV_HU);
    TEST_SETUP_EXCEPT();
    /*
     * read write in HU tests
     */
    // init ipi
    vipi = rdvipi0();
    vcpuid = rdvcpuid();
    printf("HU after write: vipi 0x%lx, vcpuid 0x%lx\n", vipi, vcpuid);
    TEST_ASSERT(
        "init vipi", 
        excpt.triggered == false && vipi == 0 && vcpuid == 0
    );

    // vipi0
    stvipi0(1 << 22);
    wrvcpuid(11);
    vipi = rdvipi0();
    vcpuid = rdvcpuid();
    printf("HU after write: vipi 0x%lx, vcpuid 0x%lx\n", vipi, vcpuid);
    TEST_ASSERT(
        "vipi0 set test", 
        excpt.triggered == false &&  vipi == 1<<22 && vcpuid == 11
    );

    // vipi1 
    stvipi1(1<<9);
    wrvcpuid(8);
    vipi = rdvipi1();
    vcpuid = rdvcpuid();
    printf("HU after write: vipi 0x%lx, vcpuid 0x%lx\n", vipi, vcpuid);
    TEST_ASSERT(
        "vipi1 set test", 
        excpt.triggered == false &&  vipi == 1<<9 && vcpuid == 8
    );

    // vipi2 
    stvipi2(1L<<59);
    wrvcpuid(77);
    vipi = rdvipi2();
    vcpuid = rdvcpuid();
    printf("HU after write: vipi 0x%lx, vcpuid 0x%lx\n", vipi, vcpuid);
    TEST_ASSERT(
        "vipi2 set test", 
        excpt.triggered == false &&  vipi == 1L<<59 && vcpuid == 77
    );

    // vipi3 
    stvipi3(1<<0);
    wrvcpuid(98);
    vipi = rdvipi3();
    vcpuid = rdvcpuid();
    printf("HU after write: vipi 0x%lx, vcpuid 0x%lx\n", vipi, vcpuid);
    TEST_ASSERT(
        "vipi3 set test", 
        excpt.triggered == false &&  vipi == 1<<0 && vcpuid == 98
    );

    /*
     * usip bit set in M tests
     */
    // vipi0 set
    wrvcpuid(22);
    for(int i = 0; i < 100; i++);
    huip = CSRR(CSR_HUIP);
    printf("HU after write: huip 0x%lx\n", huip);
    TEST_ASSERT(
        "vcpuid & vipi0 set test", 
        excpt.triggered == false &&  (huip & USIP) == USIP
    );

    // vipi0 clear
    clvipi0( ~(1L << 22) );
    CSRC(CSR_HUIP, USIP);
    for(int i = 0; i < 100; i++);
    huip = CSRR(CSR_HUIP);
    printf("HU after write: huip 0x%lx\n", huip);
    TEST_ASSERT(
       "vipi0 clear test", 
        excpt.triggered == false &&  (huip & USIP) == 0
    );

    // vipi1 set
    wrvcpuid(64 + 9);
    for(int i = 0; i < 100; i++);
    huip = CSRR(CSR_HUIP);
    printf("HU after write: huip 0x%lx\n", huip);
    TEST_ASSERT(
        "vcpuid & vipi1 set test", 
        excpt.triggered == false &&  (huip & USIP) == USIP
    );
    // vipi1 clear
    clvipi1( ~(1L << 9) );
    CSRC(CSR_HUIP, USIP);
    for(int i = 0; i < 100; i++);
    huip = CSRR(CSR_HUIP);
    printf("HU after write: huip 0x%lx\n", huip);
    TEST_ASSERT(
       "vipi1 clear test", 
        excpt.triggered == false &&  (huip & USIP) == 0
    );

    // vipi2 set
    wrvcpuid(64 * 2 + 59);
    for(int i = 0; i < 100; i++);
    huip = CSRR(CSR_HUIP);
    printf("HU after write: huip 0x%lx\n", huip);
    TEST_ASSERT(
        "vcpuid & vipi2 set test", 
        excpt.triggered == false &&  (huip & USIP) == USIP
    );
    // vipi2 clear
    clvipi2( ~(1L << 59) );
    CSRC(CSR_HUIP, USIP);
    for(int i = 0; i < 100; i++);
    huip = CSRR(CSR_HUIP);
    printf("HU after write: huip 0x%lx\n", huip);
    TEST_ASSERT(
       "vipi2 clear test", 
        excpt.triggered == false &&  (huip & USIP) == 0
    );

    // vipi3 set
    wrvcpuid(64 * 3 + 0);
    for(int i = 0; i < 100; i++);
    huip = CSRR(CSR_HUIP);
    printf("HU after write: huip 0x%lx\n", huip);
    TEST_ASSERT(
        "vcpuid & vipi3 set test", 
        excpt.triggered == false &&  (huip & USIP) == USIP
    );
    // vipi3 clear
    clvipi3( ~(1L << 0) );
    CSRC(CSR_HUIP, USIP);
    for(int i = 0; i < 100; i++);
    huip = CSRR(CSR_HUIP);
    printf("HU after write: huip 0x%lx\n", huip);
    TEST_ASSERT(
       "vipi3 clear test", 
        excpt.triggered == false &&  (huip & USIP) == 0
    );
    TEST_END();
}


DECLARE_TEST(vipi_usip_test)
{
    uint64_t huip;
    uint64_t vipi;
    uint64_t vcpuid;

    TEST_START();
    reset_state();
    goto_priv(PRIV_M);
    CSRS(mideleg,  USIP );
    goto_priv(PRIV_HS);
    // delegate USIP to U mode
    CSRS(CSR_SIDELEG,  USIP );

    goto_priv(PRIV_HU);
    CSRC(CSR_HUSTATUS, SSTATUS_UPIE_BIT | SSTATUS_UIE_BIT);
    CSRS(CSR_HUIE, USIP );
    wrvcpuid(64 * 2 + 59);
    stvipi2( 1L<<59);
    TEST_SETUP_EXCEPT();
    goto_priv(PRIV_VS);
    int i = 0; i++;
    huip = CSRR(CSR_HUIP);
    printf("huip: 0x%lx, 0x%lx, 0x%lx, 0x%lx, \n",  huip, excpt.triggered, excpt.cause, excpt.priv);
    TEST_ASSERT(
        "usip correct\n", 
        excpt.triggered == true && excpt.cause == CAUSE_USI && excpt.priv == PRIV_HU
    );
    goto_priv(PRIV_HU);
    clvipi2( ~(1L<<59) );
    TEST_END();
}

#define PLIC_BASE 0xc000000
#define VINTERRUPTS_BASE (0x1f00000 + PLIC_BASE)
#define INTERRUPT_ENABLE_BASE (0x2000 + PLIC_BASE)
#define INTERRUPT_PRIORITY_BASE (0x0 + PLIC_BASE)
#define INTERRUPT_HART_BASE (0x200000 + PLIC_BASE)
#define INTERRUPT_COMPLETE_BASE (0x200004 + PLIC_BASE)
#define INTERRUPT_CLAIM_BASE (0x200004 + PLIC_BASE)
#define INTERRUPT_PENDING_BASE (0x1000 + PLIC_BASE)

DECLARE_TEST(vplic_test) {
    TEST_START();
    uint64_t mhartid = CSRR(mhartid);
    printf("mhartid 0x%lx\n", mhartid);

    reset_state();
    // prio threshold (per-hart, step 0x1000)
    for (int i = 0; i < 0x6000; i += 0x1000) {
    	int *interrupt_hart_base = (int *)(INTERRUPT_HART_BASE + i);
        *interrupt_hart_base = 1;
    	printf("hart %x prio threshold bits 0x%x\n", i / 0x1000, *interrupt_hart_base);
    }
    
    // prio for irq src (global, step 0x4)
    // vinterrupts start from 0x80 on QEMU
    for (int i = 4 * 0x80; i < 4 * 0x80 + 8; i += 4) {
    	int *interrupt_priority_base = (int *)(INTERRUPT_PRIORITY_BASE + i);
        *interrupt_priority_base = -1;
    	printf("irq prio bits 0x%x\n", *interrupt_priority_base);
    }

    // enable for irq src (per-hart, step 0x80)
    for (int i = 0; i < 0x80 * 6; i += 0x80) {
        // cpu 0 M/S/H cpu 1 M/S/H
        // vinterrupt start 0x80
        int *interrupt_enable_base = (int *) (INTERRUPT_ENABLE_BASE + i) + (0x80 / 32);
        if (i == 0x80 * 2 || i == 0x80 * 5)
            *interrupt_enable_base = -1;
        else 
            *interrupt_enable_base = 0;
        printf("hart %d enable bits 0x%x\n", i / 0x80, *interrupt_enable_base);
    }
    
    int *vinterrupts_base_ptr = (int *) VINTERRUPTS_BASE;

    uint64_t mip = CSRR(mip);
    printf("virq before mip 0x%lx\n", mip);
    *vinterrupts_base_ptr = 1;
    printf("virq bits 0x%x\n", *vinterrupts_base_ptr);
    for (int i = 0; i < 100; i++);

    unsigned int *interrupt_pending_base = (unsigned int*)(INTERRUPT_PENDING_BASE ) + 0x80 / 32;
    printf("pending bits 0x%x\n", *interrupt_pending_base);

    mip = CSRR(mip);
    printf("virq after mip 0x%lx\n", mip);
    
    *vinterrupts_base_ptr = 0;
    printf("virq bits 0x%x\n", *vinterrupts_base_ptr);
    for (int i = 0; i < 100; i++);

    interrupt_pending_base = (unsigned int*)(INTERRUPT_PENDING_BASE ) + 0x80 / 32;
    printf("pending bits 0x%x\n", *interrupt_pending_base);

    mip = CSRR(mip);
    printf("--- virq after mip 0x%lx\n", mip);

    TEST_ASSERT("clear vinterrupt should clear mip\n",
            *interrupt_pending_base == 0 && mip == 0);
    
    *vinterrupts_base_ptr = 1;
    for (int i = 0; i < 100; i++);
    
    interrupt_pending_base = (unsigned int*)(INTERRUPT_PENDING_BASE ) + 0x80 / 32;
    printf("pending bits 0x%x\n", *interrupt_pending_base);

    mip = CSRR(mip);
    printf(">>> virq after mip 0x%lx\n", mip);
    
    CSRW(mideleg, 1 << 10);
    goto_priv(PRIV_HS);
    CSRW(CSR_HIDELEG, 1 << 10);
    CSRW(CSR_HIE, 1 << 10);
#ifdef HU_EXT
    goto_priv(PRIV_HU);
    printf("HU mode read huvip 0x%lx\n", CSRR(CSR_HUVIP));
#endif
    
    TEST_SETUP_EXCEPT();
    goto_priv(PRIV_VS);
    CSRS(sie, 1 << 9);
    CSRS(sstatus, 1 << 1);

    TEST_ASSERT("from VS to VS with CAUSE_SEI\n",
        excpt.triggered && excpt.cause == CAUSE_SEI && excpt.priv == PRIV_VS);

    // claim
    for (int i = 0; i < 0x1000 * 6; i += 0x1000) {
        // cpu 0 M/S/H cpu 1 M/S/H
    	int *interrupt_claim_base = (int *)(INTERRUPT_CLAIM_BASE + i);
    	printf("hart %d claim bits 0x%x ***0x%lx \n", i / 0x1000, *interrupt_claim_base, interrupt_claim_base);
    }
    
    goto_priv(PRIV_M);
    mip = CSRR(mip);
    printf("after claim mip 0x%lx\n", mip);
    goto_priv(PRIV_VS);

    //complete
    for (int i = 0; i < 0x1000 * 6; i += 0x1000) {
        // cpu 0 M/S cpu 1 M/S
    	int *interrupt_complete_base = (int *) (INTERRUPT_COMPLETE_BASE + i);
        *interrupt_complete_base = 0x80;
    	printf("hart %d virq bits 0x%x, pending bits 0x%x ***pending 0x%lx, complete 0x%lx\n",
                i / 0x1000, *vinterrupts_base_ptr, *interrupt_pending_base, interrupt_pending_base, interrupt_complete_base);
    }
    
    goto_priv(PRIV_M);
    mip = CSRR(mip);
    printf("after complete mip 0x%lx\n", mip);
    TEST_END();
}

DECLARE_TEST(vplic_s_test) {
    TEST_START();
    uint64_t mhartid = CSRR(mhartid);
    printf("mhartid 0x%lx\n", mhartid);

    reset_state();
    // prio threshold (per-hart, step 0x1000)
    for (int i = 0; i < 0x6000; i += 0x1000) {
    	int *interrupt_hart_base = (int *)(INTERRUPT_HART_BASE + i);
        *interrupt_hart_base = 1;
    	printf("hart %x prio threshold bits 0x%x\n", i / 0x1000, *interrupt_hart_base);
    }
    
    // prio for irq src (global, step 0x4)
    // vinterrupts start from 0x80 on QEMU
    for (int i = 4 * 0x80; i < 4 * 0x80 + 8; i += 4) {
    	int *interrupt_priority_base = (int *)(INTERRUPT_PRIORITY_BASE + i);
        *interrupt_priority_base = -1;
    	printf("irq prio bits 0x%x\n", *interrupt_priority_base);
    }

    // enable for irq src (per-hart, step 0x80)
    for (int i = 0; i < 0x80 * 6; i += 0x80) {
        // cpu 0 M/S/H cpu 1 M/S/H
        // vinterrupt start 0x80
        int *interrupt_enable_base = (int *) (INTERRUPT_ENABLE_BASE + i) + (0x80 / 32);
        if (i == 0x80 * 1 || i == 0x80 * 4)
            *interrupt_enable_base = -1;
        else 
            *interrupt_enable_base = 0;
        printf("hart %d enable bits 0x%x\n", i / 0x80, *interrupt_enable_base);
    }
    
    int *vinterrupts_base_ptr = (int *) VINTERRUPTS_BASE;

    uint64_t mip = CSRR(mip);
    printf("virq before mip 0x%lx\n", mip);
    *vinterrupts_base_ptr = 1;
    printf("virq bits 0x%x\n", *vinterrupts_base_ptr);
    for (int i = 0; i < 100; i++);

    unsigned int *interrupt_pending_base = (unsigned int*)(INTERRUPT_PENDING_BASE ) + 0x80 / 32;
    printf("pending bits 0x%x\n", *interrupt_pending_base);

    mip = CSRR(mip);
    printf("virq after mip 0x%lx\n", mip);
    
    CSRW(mideleg, 1 << 9);
    TEST_SETUP_EXCEPT();
    goto_priv(PRIV_HS);
    CSRS(sie, 1 << 9);
    CSRS(sstatus, 1 << 1);

    TEST_ASSERT("from S to S with CAUSE_SEI\n",
        excpt.triggered && excpt.cause == CAUSE_SEI && excpt.priv == PRIV_HS);

    // claim
    for (int i = 0; i < 0x1000 * 6; i += 0x1000) {
        // cpu 0 M/S/H cpu 1 M/S/H
    	int *interrupt_claim_base = (int *)(INTERRUPT_CLAIM_BASE + i);
    	printf("hart %d claim bits 0x%x\n", i / 0x1000, *interrupt_claim_base);
    }
    
    goto_priv(PRIV_M);
    mip = CSRR(mip);
    printf("after claim mip 0x%lx\n", mip);
    goto_priv(PRIV_VS);

    //complete
    for (int i = 0; i < 0x1000 * 6; i += 0x1000) {
        // cpu 0 M/S cpu 1 M/S
    	int *interrupt_complete_base = (int *) (INTERRUPT_COMPLETE_BASE + i);
        *interrupt_complete_base = 0x80;
    	printf("hart %d virq bits 0x%x, pending bits 0x%x\n",
                i / 0x1000, *vinterrupts_base_ptr, *interrupt_pending_base);
    }
    
    goto_priv(PRIV_M);
    mip = CSRR(mip);
    printf("after complete mip 0x%lx\n", mip);
    TEST_END();
}

DECLARE_TEST(vplic_m_test) {
    TEST_START();
    uint64_t mhartid = CSRR(mhartid);
    printf("mhartid 0x%lx\n", mhartid);

    reset_state();
    // prio threshold (per-hart, step 0x1000)
    for (int i = 0; i < 0x6000; i += 0x1000) {
    	int *interrupt_hart_base = (int *)(INTERRUPT_HART_BASE + i);
        *interrupt_hart_base = 1;
    	printf("hart %x prio threshold bits 0x%x\n", i / 0x1000, *interrupt_hart_base);
    }
    
    // prio for irq src (global, step 0x4)
    // vinterrupts start from 0x80 on QEMU
    for (int i = 4 * 0x80; i < 4 * 0x80 + 8; i += 4) {
    	int *interrupt_priority_base = (int *)(INTERRUPT_PRIORITY_BASE + i);
        *interrupt_priority_base = -1;
    	printf("irq prio bits 0x%x\n", *interrupt_priority_base);
    }

    // enable for irq src (per-hart, step 0x80)
    for (int i = 0; i < 0x80 * 6; i += 0x80) {
        // cpu 0 M/S/H cpu 1 M/S/H
        // vinterrupt start 0x80
        int *interrupt_enable_base = (int *) (INTERRUPT_ENABLE_BASE + i) + (0x80 / 32);
        if (i == 0x80 * 0 || i == 0x80 * 3)
            *interrupt_enable_base = -1;
        else 
            *interrupt_enable_base = 0;
        printf("hart %d enable bits 0x%x\n", i / 0x80, *interrupt_enable_base);
    }
    
    int *vinterrupts_base_ptr = (int *) VINTERRUPTS_BASE;

    uint64_t mip = CSRR(mip);
    printf("virq before mip 0x%lx\n", mip);
    *vinterrupts_base_ptr = 1;
    printf("virq bits 0x%x\n", *vinterrupts_base_ptr);
    for (int i = 0; i < 100; i++);

    unsigned int *interrupt_pending_base = (unsigned int*)(INTERRUPT_PENDING_BASE ) + 0x80 / 32;
    printf("pending bits 0x%x\n", *interrupt_pending_base);

    mip = CSRR(mip);
    printf("virq after mip 0x%lx\n", mip);
    
    TEST_SETUP_EXCEPT();
    CSRS(mie, 1 << 11);
    CSRS(mstatus, 1 << 3);

    TEST_ASSERT("from M to M with CAUSE_MEI\n",
        excpt.triggered && excpt.cause == CAUSE_MEI && excpt.priv == PRIV_M);

    // claim
    for (int i = 0; i < 0x1000 * 6; i += 0x1000) {
        // cpu 0 M/S/H cpu 1 M/S/H
    	int *interrupt_claim_base = (int *)(INTERRUPT_CLAIM_BASE + i);
    	printf("hart %d claim bits 0x%x\n", i / 0x1000, *interrupt_claim_base);
    }
    
    goto_priv(PRIV_M);
    mip = CSRR(mip);
    printf("after claim mip 0x%lx\n", mip);
    goto_priv(PRIV_VS);

    //complete
    for (int i = 0; i < 0x1000 * 6; i += 0x1000) {
        // cpu 0 M/S cpu 1 M/S
    	int *interrupt_complete_base = (int *) (INTERRUPT_COMPLETE_BASE + i);
        *interrupt_complete_base = 0x80;
    	printf("hart %d virq bits 0x%x, pending bits 0x%x\n",
                i / 0x1000, *vinterrupts_base_ptr, *interrupt_pending_base);
    }
    
    goto_priv(PRIV_M);
    mip = CSRR(mip);
    printf("after complete mip 0x%lx\n", mip);
    TEST_END();
}
