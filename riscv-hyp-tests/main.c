//#include <stdint.h>
#include <stdbool.h>
//#include <stdio.h>
//#include <stdlib.h>
#include <rvh_test.h>

DECLARE_TEST(check_csr_field_spec){

    TEST_START();

    /* this assumes machine mode */
    //check_csr_wrrd("mstatus", mstatus, (uint64_t) -1, 0x800000ca007e79aaULL);
    //check_csr_wrrd("mideleg", mideleg, (uint64_t) -1, 0x1666);
    //check_csr_wrrd("medeleg", medeleg, (uint64_t) -1, 0xb15d);
    check_csr_wrrd("mideleg", mideleg, (uint64_t)0, 0x444);
    //check_csr_wrrd("mip", mip, (uint64_t) -1, 0x6e6);
    //check_csr_wrrd("mie", mie, (uint64_t) -1, 0x1eee);
    check_csr_wrrd("mtinst", CSR_MTINST, (uint64_t) -1, (uint64_t) -1);
    check_csr_wrrd("mtval2", CSR_MTVAL2, (uint64_t) -1, (uint64_t) -1);
    //check_csr_wrrd("hstatus", CSR_HSTATUS, (uint64_t) -1, 0x30053f3e0);
    //check_csr_wrrd("hideleg", CSR_HIDELEG, (uint64_t) -1, 0x444);
    //check_csr_wrrd("hedeleg", CSR_HEDELEG, (uint64_t) -1, 0xb1ff);
    //check_csr_wrrd("hvip", CSR_HVIP, (uint64_t) -1, 0x444);
    //check_csr_wrrd("hip", CSR_HIP, (uint64_t) -1, 0x4);
    check_csr_wrrd("hie", CSR_HIE, (uint64_t) -1, 0x444);
    check_csr_wrrd("htval", CSR_HTVAL, (uint64_t) -1, (uint64_t) -1);
    check_csr_wrrd("htinst", CSR_HTINST, (uint64_t) -1, (uint64_t) -1);
    //check_csr_wrrd("hgatp", CSR_HGATP, (8ULL << 60) | (1ULL << 60)-1, 0x80000000000fffffULL);
    //check_csr_wrrd("vsstatus", CSR_VSSTATUS, (uint64_t) -1, 0x80000000000c6122ULL);
    //check_csr_wrrd("vsip", CSR_VSIP, (uint64_t) -1, 0x0);
    //check_csr_wrrd("vsie", CSR_VSIE, (uint64_t) -1, 0x0);
    //check_csr_wrrd("vstvec", CSR_VSTVEC, (uint64_t) -1, 0xffffffffffffff01ULL);
    check_csr_wrrd("vsscratch", CSR_VSSCRATCH, (uint64_t) -1, (uint64_t) -1);
    //check_csr_wrrd("vsepc", CSR_VSEPC, (uint64_t) -1, 0xfffffffffffffffeULL);
    //check_csr_wrrd("vscause", CSR_VSCAUSE, (uint64_t) -1, 0x800000000000001fULL);
    check_csr_wrrd("vstval", CSR_VSTVAL, (uint64_t) -1, 0xffffffffffffffffULL);
    //check_csr_wrrd("vsatp", CSR_VSATP, (uint64_t) -1, 0x0);
    //check_csr_wrrd("vsatp", CSR_VSATP, (8ULL << 60) | (1ULL << 60)-1, 0x80000000000fffffULL);

    TEST_END();
}

DECLARE_TEST(check_misa_h){

    TEST_START();

    uint64_t misa = CSRR(misa);
    CSRS(misa, (1ULL << 7));

    bool hyp_ext_present = CSRR(misa) & (1ULL << 7);
    TEST_ASSERT("check h bit after setting it",  hyp_ext_present, "hypervisor extensions not present");

    if(!hyp_ext_present)
        return false;

    CSRC(misa, (1ULL << 7));
    if(((CSRR(misa) & (1ULL << 7)))){
        VERBOSE("misa h bit is hardwired");
    }

    CSRW(misa, misa);

    TEST_END();
}

DECLARE_TEST(check_misa_z){

    TEST_START();

    uint64_t z = 'z' - 'a';
    printf("z : %ld\n", z);
    uint64_t misa = CSRR(misa);
    CSRS(misa, (1ULL << z));

    bool hyp_ext_present = CSRR(misa) & (1ULL << z);
    TEST_ASSERT("check i bit after setting it",  hyp_ext_present, "hypervisor extensions not present");

    if(!hyp_ext_present)
        return false;

    CSRC(misa, (1ULL << z));
    if(((CSRR(misa) & (1ULL << z)))){
        VERBOSE("misa i bit is hardwired");
    }

    CSRW(misa, misa);

    TEST_END();
}

void main() {

    INFO("risc-v hypervisor extensions tests");

    uint64_t nr_p = 0, nr_f = 0;
    uint64_t p, f;
    if(check_misa_h(&p, &f)){
        nr_p += p;
        nr_f += f;
#ifdef HU_EXT
        if(check_misa_z(&p, &f)){
            nr_p += p;
            nr_f += f;
#endif
            reset_state();
            for(int i = 0; i < test_table_size; i++) {
                test_table[i](&p, &f);
                nr_p += p;
                nr_f += f;
            }
#ifdef HU_EXT
        }
#endif
    }
    INFO("====== SUMMARY ======");
    if (nr_f == 0)
        printf(CGRN "%d tests completed. [%llu / %llu] PASSED.\n" CDFLT,
                test_table_size, nr_p, nr_p + nr_f);
    else
        printf(CRED "%d tests completed. [%llu / %llu] FAILED.\n" CDFLT,
                test_table_size, nr_f, nr_p + nr_f);
    INFO("\nend all tests");
    return 0;
}
