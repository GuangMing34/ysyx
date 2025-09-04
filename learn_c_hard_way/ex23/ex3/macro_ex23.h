#ifndef __MACRO_EX23_H__
#define __MACRO_EX23_H__

#define DUFFY_COPY_X(from, to, NUM1, NUM0) case 0##NUM1##NUM: *to++ = *from++;

#define DUFFY_COPY_0(from, to, MSB) DUFFY_COPY_X(from, to, MSB, 0)
#define DUFFY_COPY_1(from, to, MSB) DUFFY_COPY_X(from, to, MSB, 1)
#define DUFFY_COPY_2(from, to, MSB) DUFFY_COPY_X(from, to, MSB, 2)
#define DUFFY_COPY_3(from, to, MSB) DUFFY_COPY_X(from, to, MSB, 3)
#define DUFFY_COPY_4(from, to, MSB) DUFFY_COPY_X(from, to, MSB, 4)
#define DUFFY_COPY_5(from, to, MSB) DUFFY_COPY_X(from, to, MSB, 5)
#define DUFFY_COPY_6(from, to, MSB) DUFFY_COPY_X(from, to, MSB, 6)
#define DUFFY_COPY_7(from, to, MSB) DUFFY_COPY_X(from, to, MSB, 7)

#define DUFFY_COPY_MSB_7_TO_1(from, to, MSB) \
    DUFFY_COPY_7(from, to, MSB) \
    DUFFY_COPY_6(from, to, MSB) \
    DUFFY_COPY_5(from, to, MSB) \
    DUFFY_COPY_4(from, to, MSB) \
    DUFFY_COPY_3(from, to, MSB) \
    DUFFY_COPY_2(from, to, MSB) \
    DUFFY_COPY_1(from, to, MSB)

#define DUFFY_COPY_MSB_7_TO_0(from, to, MSB) \
        DUFFY_COPY_MSB_7_TO_1(from, to, MSB) \
        DUFFY_COPY_0(from, to, MSB)


#define DUFFY_COPY_2x_31_TO_1(from, to) \
        DUFFY_COPY_MSB_7_TO_0(from, to, 3) \
        DUFFY_COPY_MSB_7_TO_0(from, to, 2) \
        DUFFY_COPY_MSB_7_TO_0(from, to, 1) \
        DUFFY_COPY_MSB_7_TO_1(from, to, 0) \

#define DUFFY_COPY_31(from, to) \
    { \
        int n = (count + 31) / 32; \
        \
        switch(count % 32) { \
            case 0: do { *to++ = *from++; \
                        DUFFY_COPY_2x_31_TO_1(from, to);\
                    } while(--n > 0); \
        } \
    }

#define duffy_copy_8(from, to) \
    { \
        int n = (count + 7) / 8; \
        \
        switch(count % 8) { \
            case 0: do { *to++ = *from++; \
                        DUFFY_COPY_MSB_7_TO_1(from, to, 0);\
                    } while(--n > 0); \
        } \
    } \

#endif