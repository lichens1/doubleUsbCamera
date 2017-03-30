#include "ImageProc.h"

int yuv_tbl_ready=0;
int y1192_tbl[256];
int v1634_tbl[256];
int v833_tbl[256];
int u400_tbl[256];
int u2066_tbl[256];


METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr) cinfo->err;
	(*cinfo->err->output_message) (cinfo);
	longjmp(myerr->setjmp_buffer, 1);
}

GLOBAL(void)
jpeg_memory_src (j_decompress_ptr cinfo, void* data, unsigned long len)
{
	memory_src_ptr src;

	if (cinfo->src == NULL) {
	cinfo->src = (struct jpeg_source_mgr *)
	(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
		sizeof(memory_source_mgr));
	}

	src = (memory_src_ptr) cinfo->src;

	src->pub.init_source = memory_init_source;
	src->pub.fill_input_buffer = memory_fill_input_buffer;
	src->pub.skip_input_data = memory_skip_input_data;
	src->pub.resync_to_restart = jpeg_resync_to_restart;
	src->pub.term_source = memory_term_source;
	src->pub.bytes_in_buffer = 0;
	src->pub.next_input_byte = (JOCTET*)data;

	src->skip =0;
}

METHODDEF(void) memory_init_source (j_decompress_ptr cinfo)
{
}


METHODDEF(boolean) memory_fill_input_buffer (j_decompress_ptr cinfo)
{
	return FALSE;
}

METHODDEF(void) memory_skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
	memory_src_ptr src = (memory_src_ptr) cinfo->src;

	if (num_bytes > (long)src->pub.bytes_in_buffer) {
		src->skip = (int)(num_bytes - src->pub.bytes_in_buffer);
		src->pub.next_input_byte += src->pub.bytes_in_buffer;
		src->pub.bytes_in_buffer = 0;
	}else{
		src->pub.next_input_byte += (size_t) num_bytes;
		src->pub.bytes_in_buffer -= (size_t) num_bytes;
		src->skip=0;
	}
}

METHODDEF(void) memory_term_source (j_decompress_ptr cinfo)
{
}

static
unsigned char my_jpeg_odml_dht[0x1a4] = {
    0xff, 0xc4, 0x01, 0xa2,

    0x00, 0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,

    0x01, 0x00, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,

    0x10, 0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03, 0x05, 0x05, 0x04,
    0x04, 0x00, 0x00, 0x01, 0x7d,
    0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06,
    0x13, 0x51, 0x61, 0x07,
    0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08, 0x23, 0x42, 0xb1, 0xc1,
    0x15, 0x52, 0xd1, 0xf0,
    0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16, 0x17, 0x18, 0x19, 0x1a,
    0x25, 0x26, 0x27, 0x28,
    0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45,
    0x46, 0x47, 0x48, 0x49,
    0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65,
    0x66, 0x67, 0x68, 0x69,
    0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x83, 0x84, 0x85,
    0x86, 0x87, 0x88, 0x89,
    0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3,
    0xa4, 0xa5, 0xa6, 0xa7,
    0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba,
    0xc2, 0xc3, 0xc4, 0xc5,
    0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8,
    0xd9, 0xda, 0xe1, 0xe2,
    0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf1, 0xf2, 0xf3, 0xf4,
    0xf5, 0xf6, 0xf7, 0xf8,
    0xf9, 0xfa,

    0x11, 0x00, 0x02, 0x01, 0x02, 0x04, 0x04, 0x03, 0x04, 0x07, 0x05, 0x04,
    0x04, 0x00, 0x01, 0x02, 0x77,
    0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41,
    0x51, 0x07, 0x61, 0x71,
    0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xa1, 0xb1, 0xc1, 0x09,
    0x23, 0x33, 0x52, 0xf0,
    0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34, 0xe1, 0x25, 0xf1, 0x17,
    0x18, 0x19, 0x1a, 0x26,
    0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44,
    0x45, 0x46, 0x47, 0x48,
    0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64,
    0x65, 0x66, 0x67, 0x68,
    0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x82, 0x83,
    0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a,
    0xa2, 0xa3, 0xa4, 0xa5,
    0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8,
    0xb9, 0xba, 0xc2, 0xc3,
    0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6,
    0xd7, 0xd8, 0xd9, 0xda,
    0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf2, 0xf3, 0xf4,
    0xf5, 0xf6, 0xf7, 0xf8,
    0xf9, 0xfa
};

static
int my_jpeg_load_dht (struct jpeg_decompress_struct *info, unsigned char *dht,
              JHUFF_TBL *ac_tables[], JHUFF_TBL *dc_tables[])
{
    unsigned int length = (dht[2] << 8) + dht[3] - 2;
    unsigned int pos = 4;
    unsigned int count, i;
    int index;

    JHUFF_TBL **hufftbl;
    unsigned char bits[17];
    unsigned char huffval[256];

    while (length > 16)
    {
       bits[0] = 0;
       index = dht[pos++];
       count = 0;
       for (i = 1; i <= 16; ++i)
       {
           bits[i] = dht[pos++];
           count += bits[i];
       }
       length -= 17;

       if (count > 256 || count > length)
           return -1;

       for (i = 0; i < count; ++i)
           huffval[i] = dht[pos++];
       length -= count;

       if (index & 0x10)
       {
           index -= 0x10;
           hufftbl = &ac_tables[index];
       }
       else
           hufftbl = &dc_tables[index];

       if (index < 0 || index >= NUM_HUFF_TBLS)
           return -1;

       if (*hufftbl == NULL)
           *hufftbl = jpeg_alloc_huff_table ((j_common_ptr)info);
       if (*hufftbl == NULL)
           return -1;

       memcpy ((*hufftbl)->bits, bits, sizeof (*hufftbl)->bits);
       memcpy ((*hufftbl)->huffval, huffval, sizeof (*hufftbl)->huffval);
    }

    if (length != 0)
       return -1;

    return 0;
}




int errnoexit(const char *s)
{
	LOGE("%s error %d, %s", s, errno, strerror (errno));
	return ERROR_LOCAL;
}


int xioctl(int fd, int request, void *arg)
{
	int r;

	do r = ioctl (fd, request, arg);
	while (-1 == r && EINTR == errno);

	return r;
}

int checkCamerabase(){
	struct stat st;
	int i;
	int start_from_4 = 1;

	/* if video0..video3 exists, camerabase=4, otherwise, camrerabase = 0 */
	for(i=0 ; i<4 ; i++){
		sprintf(dev_name[0],"/dev/video%d",i);
		if (-1 == stat (dev_name[0], &st)) {
			start_from_4 &= 0;
		}else{
			start_from_4 &= 1;
		}
	}

	if(start_from_4){
		return 4;
	}else{
		return 0;
	}
}

int opendevice(int camid, int videoid)
{
	struct stat st;

	sprintf(dev_name[camid],"/dev/video%d",videoid);
	LOGE("opening%s", dev_name[camid]);

	if (-1 == stat (dev_name[camid], &st)) {
		LOGE("Cannot identify '%s': %d, %s", dev_name[camid], errno, strerror (errno));
		return ERROR_LOCAL;
	}

	if (!S_ISCHR (st.st_mode)) {
		LOGE("%s is no device", dev_name[camid]);
		return ERROR_LOCAL;
	}

	fd[camid] = open (dev_name[camid], O_RDWR | O_NONBLOCK, 0);

	if (-1 == fd[camid]) {
		LOGE("Cannot open '%s': %d, %s", dev_name[camid], errno, strerror (errno));
		return ERROR_LOCAL;
	}
}

int initdevice(int i)
{
	struct v4l2_capability cap;
	struct v4l2_cropcap cropcap;
	struct v4l2_crop crop;
	struct v4l2_format fmt;
	unsigned int min;

	int ret;

	if (-1 == xioctl (fd[i], VIDIOC_QUERYCAP, &cap)) {
		if (EINVAL == errno) {
			LOGE("%s is no V4L2 device", dev_name[i]);
			return ERROR_LOCAL;
		} else {
			return errnoexit ("VIDIOC_QUERYCAP");
		}
	}

	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		LOGE("%s is no video capture device", dev_name[i]);
		return ERROR_LOCAL;
	}


	if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
		LOGE("%s does not support streaming i/o", dev_name[i]);
		return ERROR_LOCAL;
	}

	CLEAR (cropcap);

	cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (0 == xioctl (fd[i], VIDIOC_CROPCAP, &cropcap)) {
		crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		crop.c = cropcap.defrect;

		if (-1 == xioctl (fd[i], VIDIOC_S_CROP, &crop)) {
			switch (errno) {
				case EINVAL:
					break;
				default:
					break;
			}
		}
	} else {
	}


	CLEAR (fmt);

	fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width       = IMG_WIDTH; 
	fmt.fmt.pix.height      = IMG_HEIGHT;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

	if (-1 == xioctl (fd[i], VIDIOC_S_FMT, &fmt))
		return errnoexit ("VIDIOC_S_FMT");

	min = fmt.fmt.pix.width * 2;
	if (fmt.fmt.pix.bytesperline < min)
		fmt.fmt.pix.bytesperline = min;
	min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
	if (fmt.fmt.pix.sizeimage < min)
		fmt.fmt.pix.sizeimage = min;

	return  initmmap (i);

}

int initmmap(int i)
{
	struct v4l2_requestbuffers req;

	CLEAR (req);

	req.count               = 4;
	req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory              = V4L2_MEMORY_MMAP;

	if (-1 == xioctl (fd[i], VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			LOGE("%s does not support memory mapping", dev_name[i]);
			return ERROR_LOCAL;
		} else {
			return errnoexit ("VIDIOC_REQBUFS");
		}
	}

	if (req.count < 2) {
		LOGE("Insufficient buffer memory on %s", dev_name[i]);
		return ERROR_LOCAL;
	}

	buffers[i] = calloc (req.count, sizeof(struct buffer));

	if (!buffers[i]) {
		LOGE("Out of memory");
		return ERROR_LOCAL;
	}

	for (n_buffers[i] = 0; n_buffers[i] < req.count; ++n_buffers[i]) {
		struct v4l2_buffer buf;

		CLEAR (buf);

		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = n_buffers[i];

		if (-1 == xioctl (fd[i], VIDIOC_QUERYBUF, &buf))
			return errnoexit ("VIDIOC_QUERYBUF");

		buffers[i][n_buffers[i]].length = buf.length;
		buffers[i][n_buffers[i]].start =
		mmap (NULL,
			buf.length,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			fd[i], buf.m.offset);

		if (MAP_FAILED == buffers[i][n_buffers[i]].start)
			return errnoexit ("mmap");
	}

	return SUCCESS_LOCAL;
}

int startcapturing(int camid)
{
	unsigned int i;
	enum v4l2_buf_type type;

	for (i = 0; i < n_buffers[camid]; ++i) {
		struct v4l2_buffer buf;

		CLEAR (buf);

		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = i;

		if (-1 == xioctl (fd[camid], VIDIOC_QBUF, &buf)){
			LOGE("in startcapturing");
			return errnoexit ("VIDIOC_QBUF");
		}
	}
                
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == xioctl (fd[camid], VIDIOC_STREAMON, &type)){
		LOGE("camid=%d",camid);
		return errnoexit ("VIDIOC_STREAMON");
	}

	return SUCCESS_LOCAL;
}

int readframeonce(int i)
{
	for (;;) {
		fd_set fds;
		struct timeval tv;
		int r;

		FD_ZERO (&fds);
		FD_SET (fd[i], &fds);

		// Timeout
		tv.tv_sec = 2;
		tv.tv_usec = 0;

		r = select (fd[i] + 1, &fds, NULL, NULL, &tv);

		if (-1 == r) {
			if (EINTR == errno)
				continue;
			return errnoexit ("select");
		}

		if (0 == r) {
			LOGE("select timeout");
			return ERROR_LOCAL;
		}

		if (readframe (i)==1)
			break;
	}

	return SUCCESS_LOCAL;

}


void processimage (int i, const void *p, int l)
{

	struct jpeg_decompress_struct mycinfo;
	struct my_error_mgr myjerr;
	JSAMPARRAY jpegbuffer=NULL;
	int row_stride;

	mycinfo.err = jpeg_std_error(&myjerr.pub);
	myjerr.pub.error_exit = my_error_exit;
	if (setjmp(myjerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&mycinfo);
		return ;/*exit(0);*/
	}
	jpeg_create_decompress(&mycinfo);

	jpeg_memory_src(&mycinfo, p, l) ;

	((memory_source_mgr *)mycinfo.src)->pub.next_input_byte = (JOCTET*)p;
	((memory_source_mgr *)mycinfo.src)->pub.bytes_in_buffer = l;

	jpeg_read_header(&mycinfo, TRUE);

	mycinfo.out_color_space = JCS_RGB;
	mycinfo.dct_method = JDCT_IFAST;
	mycinfo.jpeg_color_space = JCS_YCbCr;

	my_jpeg_load_dht( &mycinfo, 
		my_jpeg_odml_dht, 
		mycinfo.ac_huff_tbl_ptrs, 
		mycinfo.dc_huff_tbl_ptrs ); 

	jpeg_start_decompress(&mycinfo);

	row_stride = mycinfo.image_width * mycinfo.num_components;

	if(jpegbuffer==NULL){
		jpegbuffer = (*mycinfo.mem->alloc_sarray)
			((j_common_ptr) &mycinfo, JPOOL_IMAGE, row_stride, 1);
	}

	int y = 0;
	int *outp=rgb[i];
	while ( mycinfo.output_scanline < mycinfo.image_height) {

		jpeg_read_scanlines(&mycinfo, jpegbuffer, 1);

		int xx;
		int x3;

		for(xx = 0, x3 = 0; xx < IMG_WIDTH && x3 < row_stride; xx++, x3 += 3)
		{
			outp[y+xx] = 0xff000000 | jpegbuffer[0][x3 + 2]<<16
				| jpegbuffer[0][x3 + 1]<<8 | jpegbuffer[0][x3 + 0];
		}

		y+=IMG_WIDTH;
	}

	jpeg_finish_decompress(&mycinfo);
	jpeg_destroy_decompress(&mycinfo);
	
}

int readframe(int i)
{

	struct v4l2_buffer buf;

	CLEAR (buf);

	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl (fd[i], VIDIOC_DQBUF, &buf)) {
		switch (errno) {
			case EAGAIN:
				return 0;
			case EIO:
			default:
				return errnoexit ("VIDIOC_DQBUF");
		}
	}

	assert (buf.index < n_buffers[i]);

	processimage (i, buffers[i][buf.index].start, buffers[i][buf.index].length);

	if (-1 == xioctl (fd[i], VIDIOC_QBUF, &buf)){
		LOGE("in readframe");
		return errnoexit ("VIDIOC_QBUF");
	}

	return 1;
}

int stopcapturing(int i)
{
	enum v4l2_buf_type type;

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == xioctl (fd[i], VIDIOC_STREAMOFF, &type))
		return errnoexit ("VIDIOC_STREAMOFF");

	return SUCCESS_LOCAL;

}

int uninitdevice(int camid)
{
	unsigned int i;

	for (i = 0; i < n_buffers[camid]; ++i)
		if (-1 == munmap (buffers[camid][i].start, buffers[camid][i].length))
			return errnoexit ("munmap");

	free (buffers[camid]);

	return SUCCESS_LOCAL;
}

int closedevice(int i)
{
	if (-1 == close (fd[i])){
		fd[i] = -1;
		return errnoexit ("close");
	}

	fd[i] = -1;
	return SUCCESS_LOCAL;
}

void Java_com_camera_simpledoublewebcams2_CameraPreview_pixeltobmp( JNIEnv* env,jobject thiz,jobject bitmap1, jobject bitmap2){

	jboolean bo;
	int j;
	for(j=0 ; j<CAMERANUM ; j++){
		if(!rgb[j]){
			continue;
		}

		AndroidBitmapInfo  info;
		void*              pixels;
		int                ret;
		int i;
		int *colors;

		int width = IMG_WIDTH;
		int height = IMG_HEIGHT;
		
		if(j==0){
			if(bitmap1==NULL) continue;
			if ((ret = AndroidBitmap_getInfo(env, bitmap1, &info)) < 0) {
				LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
				return;
			}

			if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
				LOGE("Bitmap format is not RGBA_8888 !");
				return;
			}

			if ((ret = AndroidBitmap_lockPixels(env, bitmap1, &pixels)) < 0) {
				LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
			}
		}else{
			if(bitmap2==NULL) continue;
			if ((ret = AndroidBitmap_getInfo(env, bitmap2, &info)) < 0) {
				LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
				return;
			}

			if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
				LOGE("Bitmap format is not RGBA_8888 !");
				return;
			}

			if ((ret = AndroidBitmap_lockPixels(env, bitmap2, &pixels)) < 0) {
				LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
			}
		}

		colors = (int*)pixels;
		int *lrgb = &rgb[j][0];

		for(i=0 ; i<width*height ; i++){
			*colors++ = *lrgb++;
		}

		if(j==0){
			AndroidBitmap_unlockPixels(env, bitmap1);
		}else{
			AndroidBitmap_unlockPixels(env, bitmap2);
		}
	}
}

jint Java_com_camera_simpledoublewebcams2_CameraPreview_prepareCamera(JNIEnv* env,jobject thiz, jint videoid){

	int ret[CAMERANUM];
	int i;
		
	if(camerabase<0){
		camerabase = checkCamerabase();
	}
		
	for(i=0 ; i<CAMERANUM ; i++){
		ret[i] = opendevice(i, camerabase+videoid+i);

		if(ret[i]  != ERROR_LOCAL){
			ret[i]  = initdevice(i);
		}
		if(ret[i]  != ERROR_LOCAL){
			ret[i] = startcapturing(i);
		}

		if(ret[i]  != ERROR_LOCAL){
			rgb[i] = (int *)malloc(sizeof(int) * (IMG_WIDTH*IMG_HEIGHT));
		}
	}
		
	if(ret[0]==ERROR_LOCAL || ret[1]==ERROR_LOCAL ){
		return ret[1]*2+ret[0];
	}else{
		return SUCCESS_LOCAL ;
	}
}	

jint Java_com_camera_simpledoublewebcams2_CameraPreview_prepareCameraWithBase( JNIEnv* env,jobject thiz, jint videoid, jint videobase){
	
	int ret;

	camerabase = videobase;

	return Java_com_camera_simpledoublewebcams2_CameraPreview_prepareCamera(env,thiz,videoid);

}

void Java_com_camera_simpledoublewebcams2_CameraPreview_processCamera( JNIEnv* env,jobject thiz){

	int i;
	for(i=0 ; i<CAMERANUM ; i++){
		if(fd[i]!=-1){
			readframeonce(i);
		}
	}
}


void Java_com_camera_simpledoublewebcams2_CameraPreview_stopCamera(JNIEnv* env,jobject thiz){

	int i;
	for(i=0 ; i<CAMERANUM ; i++){

		if(fd[i]!=-1) stopcapturing (i);

		if(fd[i]!=-1) uninitdevice (i);

		if(fd[i]!=-1) closedevice (i);
        
		fd[i] =-1;

		if(rgb[i]) free(rgb[i]);
	}
}

void Java_com_camera_simpledoublewebcams2_CameraPreview_processRBCamera( JNIEnv* env,jobject thiz, jint lrmode){
	int i;
	int red = 1-lrmode;
	int blue = lrmode;
	
	int *lrgb0 = rgb[0];
	int *lrgb_red = NULL;
	int *lrgb_blue = NULL;
	if(rgb[red]) lrgb_red = rgb[red];
	if(rgb[blue]) lrgb_blue = rgb[blue];
	for(i=0 ; i<IMG_WIDTH*IMG_HEIGHT ; i++){
		
		if(rgb[red] && rgb[blue]){
			*lrgb_red = *lrgb_red & 0xff0000ff;
			*lrgb_blue = *lrgb_blue & 0xffffff00;
			*lrgb0 = *lrgb_red | *lrgb_blue;
			lrgb_red++;
			lrgb_blue++;
			lrgb0++;
		}else if(rgb[red]){
			*lrgb_red = *lrgb_red & 0xff0000ff;
			*lrgb0 = *lrgb_red;
			lrgb_red++;
			lrgb0++;
		}else if(rgb[blue]){
			*lrgb_blue= *lrgb_blue & 0xff0000ff;
			*lrgb0 = *lrgb_blue;
			lrgb_blue++;
			lrgb0++;
		}
	}

}