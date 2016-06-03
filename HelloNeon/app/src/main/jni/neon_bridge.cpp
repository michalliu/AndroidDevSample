#include <jni.h>
#include <arm_neon.h>
#include <android/log.h>
#include <malloc.h>


const int DATA_SIZE = 1920*1080;

void test() {
    int *testSet1 = (int*)malloc(sizeof(int)*DATA_SIZE);

    for(uint32_t i = 0; i<DATA_SIZE; i++) {
        testSet1[i] = i;
    }


    for (uint32_t i=0; i<DATA_SIZE/4/2; i++) {
        int32_t *src = testSet1+i*4;
        int32_t *dest = testSet1+DATA_SIZE - 4*(i+1);
        int32x4_t tmp = vld1q_dup_s32(src);
        int32x4_t destData = vld1q_dup_s32(dest);
        int32x4_t rDestData = vrev64q_s32(destData);
        vst1q_s32(src, rDestData);
        vst1q_s32(dest, tmp);
    }

    __android_log_print(ANDROID_LOG_DEBUG, "NEON", "last number is %d", testSet1[DATA_SIZE-1]);

    free(testSet1);
}

static JNINativeMethod gMethods[] = {
        {"makeTest",       "()V",            (void *)test}
};

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;
    //判断一下JNI的版本
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        __android_log_print(ANDROID_LOG_ERROR, "NEON"," JNI init failed.. ");
        goto bail;
    }

    __android_log_print(ANDROID_LOG_DEBUG, "NEON"," JNI init succeeded.. ");

   if(JNI_OK != env->RegisterNatives(env->FindClass("com/tencent/helloneon/MainActivity"), gMethods, 1))
   {
       __android_log_print(ANDROID_LOG_ERROR, "NEON"," method register failed..");
   }

    result = JNI_VERSION_1_4;

    bail:
    return result;
}

JNIEXPORT void JNICALL
Java_com_tencent_helloneon_MainActivity_makeTest__(JNIEnv *env, jclass type) {

    test();

}