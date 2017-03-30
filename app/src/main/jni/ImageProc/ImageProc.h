#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>

#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>          /* for videodev2.h */

#include <linux/videodev2.h>

#include <jpeglib.h>
#include <jerror.h>
#include <setjmp.h>

#define  LOG_TAG    "DoubleWebCam"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define abs_mcr(x) ((x)>0 ? (x) : -(x))


#define CLEAR(x) memset (&(x), 0, sizeof (x))
#define CAMERANUM 2

struct buffer {
        void *                  start;
        size_t                  length;
};


static int camerabase=-1;
static char            dev_name[CAMERANUM][16];
static int              fd[CAMERANUM]              = {-1,-1};
struct buffer *         buffers[CAMERANUM]         = {NULL,NULL};
static unsigned int     n_buffers[CAMERANUM]       = {0,0};

static int *rgb[CAMERANUM] = {NULL,NULL};

#define IMG_WIDTH 640
#define IMG_HEIGHT 480

#define ERROR_LOCAL -1
#define SUCCESS_LOCAL 0

/* for libjpeg */
typedef struct {
  struct jpeg_source_mgr pub;/* public fields */
  int skip;
} memory_source_mgr;
typedef memory_source_mgr *memory_src_ptr;

struct my_error_mgr {
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void) my_error_exit (j_common_ptr cinfo);
GLOBAL(void) jpeg_memory_src (j_decompress_ptr cinfo, void* data, unsigned long len);
METHODDEF(void) memory_init_source (j_decompress_ptr cinfo);
METHODDEF(boolean) memory_fill_input_buffer (j_decompress_ptr cinfo);
METHODDEF(void) memory_skip_input_data (j_decompress_ptr cinfo, long num_bytes);
METHODDEF(void) memory_term_source (j_decompress_ptr cinfo);
/* end of libjpeg */


int errnoexit(const char *s);

int xioctl(int fd, int request, void *arg);

int opendevice(int camid, int videoid);
int initdevice(int i);
int initmmap(int i);
int startcapturing(int i);

int readframeonce(int i);
int readframe(int i);
void processimage (int camid, const void *p, int l);

int stopcapturing(int i);
int uninitdevice(int i);
int closedevice(int i);

void Java_com_camera_simpledoublewebcams2_CameraPreview_pixeltobmp(JNIEnv* env,jobject thiz, jobject bitmap1, jobject bitmap2);
jint Java_com_camera_simpledoublewebcams2_CameraPreview_prepareCamera(JNIEnv* env,jobject thiz, jint videoid);
jint Java_com_camera_simpledoublewebcams2_CameraPreview_prepareCameraWithBase(JNIEnv* env,jobject thiz, jint videoid, jint videobase);
void Java_com_camera_simpledoublewebcams2_CameraPreview_processCamera(JNIEnv* env,jobject thiz);
void Java_com_camera_simpledoublewebcams2_CameraPreview_stopCamera(JNIEnv* env,jobject thiz);
void Java_com_camera_simpledoublewebcams2_CameraPreview_processRBCamera(JNIEnv* env,jobject thiz, jint lrmode);
