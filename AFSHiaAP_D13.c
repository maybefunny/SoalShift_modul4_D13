#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/statfs.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

#define mymemset(x) memset(x, 0, sizeof(x));
#define  DEFAULT_CHUNK  262144  /* 256k */

const char set1[]="qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";
char set2[]="zi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0qE1~ YMUR2\"`hNIdP";
static unsigned char encTable[256];
static unsigned char decTable[256];
static const char *dirpath = "/home/mungkin/shift4";
int status=0;

char* formatdate(char* str, time_t val);
bool ext_match(const char *name, const char *ext);
int check_file(char *path);
bool isSplit(const char *name);
int getFileName(char myFile[]);
int remove_directory(const char *path);
void encrypt(char message[]);
void decrypt(char message[]);
void* myJoin(void *arg);

static int xmp_getattr(const char *path, struct stat *stbuf);
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,off_t offset, struct fuse_file_info *fi);
static int xmp_read(const char *path, char *buf, size_t size, off_t offset);
static int xmp_mkdir(const char *path, mode_t mode);
static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi);
static int xmp_release(const char *path, struct fuse_file_info *fi);
static int xmp_chmod(const char *path, mode_t mode);

void xmp_init(struct fuse_conn_info *conn, struct fuse_config *cfg){
    size_t q;
    int s;
    for(s = 0; s < 256; s++){
		encTable[s] = s;
		decTable[s] = s;
	}
	for(q = 0; set1[q] !='\0'; q++){
		if(set2[q] != set1[q]) encTable[(int)set1[q]] = set2[q];
		if(set2[q] != set1[q]) decTable[(int)set2[q]] = set1[q];
	}

    DIR* dir = opendir("/home/mungkin/shift4/g[xO#Y");
    int r = -1;
    if (dir){
        closedir(dir);
    }else{
        mkdir("/home/mungkin/shift4/g[xO#Y", 0777);
    }
    dir = opendir("/home/mungkin/shift4/");

    if(dir){
        struct dirent *p;
        r = 0;
        int i=0;
        pthread_t tid[1000];

        while (!r && (p=readdir(dir))){
            if(ext_match(p->d_name, "`PPP")){
                status=1;
                pthread_create(&(tid[i++]), NULL, myJoin, (void *) p->d_name);
                while(status);
            }
        }

        while(--i){
            pthread_join(tid[i],NULL);
        }
        closedir(dir);
    }
}
void xmp_destroy(void *private_data){
    remove_directory("/home/mungkin/shift4/g[xO#Y");
}

#ifdef HAVE_SETXATTR
static int xmp_setxattr(const char *path, const char *name, const char *value,size_t size, int flags){
    printf("DICOBA GAN!!!! %s-- %s\n", path, name);
	int res = lsetxattr(path, name, value, size, flags);
	if (res == -1)
		return -errno;
	return 0;
}

static int xmp_getxattr(const char *path, const char *name, char *value,size_t size){
	int res = lgetxattr(path, name, value, size);
	if (res == -1)
		return -errno;
	return res;
}

static int xmp_listxattr(const char *path, char *list, size_t size){
	int res = llistxattr(path, list, size);
	if (res == -1)
		return -errno;
	return res;
}

static int xmp_removexattr(const char *path, const char *name){
	int res = lremovexattr(path, name);
	if (res == -1)
		return -errno;
	return 0;
}
#endif /* HAVE_SETXATTR */

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
    .mkdir      = xmp_mkdir,
    .create     = xmp_create,
    .release    = xmp_release,
    .chmod      = xmp_chmod,
    .init       = xmp_init,
    .destroy    = xmp_destroy,
#ifdef HAVE_SETXATTR
	.setxattr	= xmp_setxattr,
	.getxattr	= xmp_getxattr,
	.listxattr	= xmp_listxattr,
	.removexattr	= xmp_removexattr,
#endif
    /* ASKDHASHDJKASHDKJASDHASKJDHA
    ASDHJGASHJDGASHJD
    ASHJDGASHJD
    AASJDHHKASHDJKAHSKDJ
    ASJKDHASGDHJASGHJFANMD
    asjhjdjkashdjkahsdkjhasjkdhak */
};

int main(int argc, char *argv[]){
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}

static int xmp_getattr(const char *path, struct stat *stbuf){
  int res;
	char fpath[1000];
    char fname[1000];
    strcpy(fname, path);
    encrypt(fname);
    mymemset(fpath);
	sprintf(fpath,"%s%s",dirpath,fname);
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,off_t offset, struct fuse_file_info *fi){
    char fpath[1000];
	if(strcmp(path,"/") == 0){
		path=dirpath;
		sprintf(fpath,"%s",path);
	}else{
        char fname[1000];
        strcpy(fname, path);
        encrypt(fname);
        mymemset(fpath);
        sprintf(fpath, "%s%s",dirpath,fname);
    }
	int res = 0;

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
    chdir(fpath);
	if (dp == NULL) return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		stat(de->d_name, &st);

        struct passwd *pw = getpwuid(st.st_uid);
        struct group *gr = getgrgid(st.st_gid);

        if(!strcmp(pw->pw_name, "chipset")||!strcmp(pw->pw_name, "ic_controller")){
            if(!strcmp(gr->gr_name,"rusak")){
                if(!check_file(de->d_name)){
                    FILE *myfile;
                    char fdata[1000];
                    char ftime[37];
                    
                    mymemset(fdata);
                    sprintf(fdata, "%s %d %d %s\n", de->d_name, st.st_uid, st.st_gid, formatdate(ftime, st.st_atime));
                    myfile = fopen("/home/mungkin/shift4/V[EOr[c[Y`HDH", "a");
                    fprintf(myfile, "%s", fdata);
                    fclose(myfile);
                    
                    if (S_ISDIR(st.st_mode)){
                        remove_directory(de->d_name);
                    }else{
                        remove(de->d_name);
                    }
                }
            }
        }

        char fname[256];
        strcpy(fname, de->d_name);
        if(strcmp(de->d_name,".") && strcmp(de->d_name, "..")) decrypt(fname);
        if(isSplit(fname)) continue;
		res = (filler(buf, fname, &st, 0));
			if(res!=0) break;
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset){
    char fpath[1000];
	if(strcmp(path,"/") == 0){
		path=dirpath;
		sprintf(fpath,"%s",path);
	}else{
        char fname[1000];
        strcpy(fname, path);
        encrypt(fname);
        mymemset(fpath);
        sprintf(fpath, "%s%s",dirpath,fname);
    }
	int res = 0;
    int fd = 0 ;

	fd = open(fpath, O_RDONLY);
	if (fd == -1) return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1) res = -errno;

	close(fd);
	return res;
}

static int xmp_mkdir(const char *path, mode_t mode){
    int res;
    
    char fname[1000], fpath[1000];
    strcpy(fname,path);
    encrypt(fname);
    mymemset(fpath);
    sprintf(fpath, "%s%s",dirpath,fname);
    
    int i=strlen(fpath);

    while(fpath[--i]!='/');
    while(fpath[--i]!='/');
    i++;

    char strYOUTUBER[]="@ZA>AXio";
    char strSEMENTARA[]="@ZA>AXio";
    
    mymemset(strSEMENTARA);
    strncat(strSEMENTARA,(fpath+i),8*sizeof(char));

    if(!strcmp(strSEMENTARA, strYOUTUBER)){
        res = mkdir(fpath, 0750);
    }else{
        res = mkdir(fpath, mode);
    }

    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi){

    (void) fi;

    int res;

    char fname[1000], fpath[1000];
    strcpy(fname,path);
    encrypt(fname);
    mymemset(fpath);
    sprintf(fpath, "%s%s",dirpath,fname);

    int i=strlen(fpath);

    while(fpath[--i]!='/');
    while(fpath[--i]!='/');
    i++;

    char strYOUTUBER[]="@ZA>AXio";
    char strSEMENTARA[]="@ZA>AXio";
    
    mymemset(strSEMENTARA);
    strncat(strSEMENTARA,(fpath+i),8*sizeof(char));

    if(!strcmp(strSEMENTARA, strYOUTUBER)){
        strcat(fpath,"`[S%");
        res = creat(fpath, 0640);
    }else{
        res = creat(fpath, mode);
    }

    if(res == -1)
	return -errno;

    close(res);

    return 0;
}

static int xmp_release(const char *path, struct fuse_file_info *fi){
	(void) path;
	(void) fi;
	return 0;
}

static int xmp_chmod(const char *path, mode_t mode){
	int res;

    char fname[1000], fpath[1000];
    strcpy(fname,path);
    encrypt(fname);
    mymemset(fpath);
    sprintf(fpath, "%s%s",dirpath,fname);

    if(ext_match(fpath,"`[S%")){
        system("zenity --warning --text='File ekstensi iz1 tidak boleh diubah permissionnya.'");
        return 0;
    }

	res = chmod(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

char* formatdate(char* str, time_t val){
        strftime(str, 36, "%d.%m.%Y %H:%M:%S", localtime(&val));
        return str;
}

bool ext_match(const char *name, const char *ext){
	size_t nl = strlen(name), el = strlen(ext);
	return nl >= el && !strcmp(name + nl - el, ext);
}

int check_file(char *path){
    FILE *file_type;
    char command[] = "/usr/bin/file";
    char command_to_execute[512];
    char check[512];
    int correct = 0;

    mymemset(command_to_execute);
    sprintf(command_to_execute,"%s %s",command,path);
    file_type = popen(command_to_execute,"r");
    if(file_type == NULL)
    {
    return correct;
    }
    fgets(check,512,file_type);
    char *pointer;
    pointer = strstr(check,"ASCII"); 
    if(pointer != NULL)
    correct = 1;
    pointer = strstr(check,"UTF");
    if(pointer != NULL)    
    correct = 1;          
    pclose(file_type);
    return correct;
}

bool isSplit(const char *name){
	size_t nl = strlen(name), el = 3;
    int i;
    if((name+nl-el-1)[0]!='.'){
        return false;
    }else{
        for(i=0;i<3;i++){
            if(!isdigit((name+nl-el)[i])) return false;
        }
    }
	return true;
}

int remove_directory(const char *path){
   DIR *d = opendir(path);
   size_t path_len = strlen(path);
   int r = -1;

   if (d){
      struct dirent *p;
      r = 0;

      while (!r && (p=readdir(d))){
          int r2 = -1;
          char *buf;
          size_t len;

          /* Skip the names "." and ".." as we don't want to recurse on them. */
          if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")){
             continue;
          }

          len = path_len + strlen(p->d_name) + 2; 
          buf = malloc(len);

          if (buf){
             struct stat statbuf;
             snprintf(buf, len, "%s/%s", path, p->d_name);
             if (!stat(buf, &statbuf)){
                if (S_ISDIR(statbuf.st_mode)){
                   r2 = remove_directory(buf);
                }else{
                   r2 = unlink(buf);
                }
             }
             free(buf);
          }
          r = r2;
      }
      closedir(d);
   }

   if (!r){
      r = rmdir(path);
   }

   return r;
}

int getFileName(char myFile[]){
    int i=strlen(myFile),j=0;
    while(myFile[--i]!='/');
    while(myFile[i++]!='\0') myFile[j++]=myFile[i];
    return j;
}

int copy_file(const char *target, const char *source, const size_t chunk){
    const size_t size = (chunk > 0) ? chunk : DEFAULT_CHUNK;
    char        *data, *ptr, *end;
    ssize_t      bytes;
    int          ifd, ofd, err;

    /* NULL and empty file names are invalid. */
    if (!target || !*target || !source || !*source)
        return EINVAL;

    ifd = open(source, O_RDONLY);
    if (ifd == -1){
        return errno;}

    /* Create output file; fail if it exists (O_EXCL): */
    ofd = open(target, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (ofd == -1) {
        err = errno;
        close(ifd);
        return err;
    }

    /* Allocate temporary data buffer. */
    data = malloc(size);
    if (!data) {
        close(ifd);
        close(ofd);
        /* Remove output file. */
        unlink(target);
        return ENOMEM;
    }

    /* Copy loop. */
    while (1) {

        /* Read a new chunk. */
        bytes = read(ifd, data, size);
        if (bytes < 0) {
            if (bytes == -1)
                err = errno;
            else
                err = EIO;
            free(data);
            close(ifd);
            close(ofd);
            unlink(target);
            return err;
        } else
        if (bytes == 0)
            break;

        /* Write that same chunk. */
        ptr = data;
        end = data + bytes;
        while (ptr < end) {

            bytes = write(ofd, ptr, (size_t)(end - ptr));
            if (bytes <= 0) {
                if (bytes == -1)
                    err = errno;
                else
                    err = EIO;
                free(data);
                close(ifd);
                close(ofd);
                unlink(target);
                return err;
            } else
                ptr += bytes;
        }
    }

    free(data);

    err = 0;
    if (close(ifd))
        err = EIO;
    if (close(ofd))
        err = EIO;
    if (err) {
        unlink(target);
        return err;
    }

    return 0;
}

void encrypt(char message[]){
    int i;
	for(i=0;message[i]!=0;i++) message[i]=encTable[(int)message[i]];
}

void decrypt(char message[]){
    int i;
	for(i=0;message[i]!=0;i++) message[i]=decTable[(int)message[i]];
}

void* myJoin(void *arg){
    char* fname = (char *) arg;
    status=0;

    int i;
    char target[4096], source[4096];

    fname[strlen(fname)-4]=0;
    mymemset(target);
    sprintf(target, "/home/mungkin/shift4/g[xO#Y/%s", fname);

    strcat(fname,"`");
    for(i=0;;i++){
        char mynum[1000];
        mymemset(mynum);
        sprintf(mynum,"%03d",i);
        encrypt(mynum);

        mymemset(source);
        sprintf(source,"/home/mungkin/shift4/%s%s", fname, mynum);
        if(copy_file(target,source, 0)) break;
    }
}