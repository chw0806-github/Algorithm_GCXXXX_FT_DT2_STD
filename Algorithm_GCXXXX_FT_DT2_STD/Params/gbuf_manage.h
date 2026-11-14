#ifndef GBUF_MANAGE_H
#define GBUF_MANAGE_H

#include <QString>
#include <iterator>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>

typedef  unsigned char uchar;

enum Memory_Area{Result_area,Alculate_area};

enum Buf_type{Buf_uchar_8=8,Buf_uchar_10=10,
              Buf_uchar_16=16,Buf_short=16,
              Buf_int=32,Buf_float=32,
              Buf_double=64};


class Memory_Manage
{
public:
    Memory_Manage()=default;

    Memory_Manage(uchar * start,long int size_);

    bool Create(uchar * start,long int size_);

    bool new_buf_(uchar ** ptr,long int size_);

    bool free_buf_(uchar ** ptr);

    long Remaining_memory() const {return size-now_loca;}

    uchar** top_pointer()
    {
        if(manages.size()>0)
            return manages.top().first;
        else
            return NULL;
    }

    bool _Transfer_buf(int x,int y );
    void f_Transfer_buf( );
private:

    uchar *start_ptr;
    long int size;
    long int now_loca;

    std::stack< std::pair<uchar **,int> > manages;
};

class Pointer_status
{
public:
    Pointer_status()=default;
    Pointer_status(Memory_Area status_, Buf_type Buftype_size_,
                   int frame_cnt_);

    Memory_Area show_memory_status()const {return memory_status;}
    Buf_type show_Buftype()const {return Buftype;}
    Memory_Area show_frame_cnt()const {return memory_status;}

private:
    Memory_Area memory_status;
    Buf_type Buftype;
    int frame_cnt;
};

class Gbuf_Manage
{
public:
    Gbuf_Manage()=default;
    Gbuf_Manage(uchar *ptr, int width_, int height_,int frame_used_,
                int frame_all_, int buf_type_=2);
    Gbuf_Manage(Gbuf_Manage& ) =delete;

    bool Create(uchar *ptr, int width_, int height_,int frame_used_,
                int frame_all_, int buf_type_=2);

    bool new_buf_(uchar** ptr,Memory_Area Area,Buf_type type,
                  int width,int height,int famer_cnt);

    bool free_buf_(uchar **ptr);

    void un_locked(){is_locked=false;}
    void locked(){is_locked=true;}
    bool locked_status()const {return is_locked;}

    bool Free_all_buf();
    bool Free_result_buf();
    bool Free_alculate_buf();

private:

    Memory_Manage g_buf_result_s;
    Memory_Manage g_buf_alculate_s;

    std::vector<uchar *> pointer_vecs;
    std::map<uchar **,Pointer_status> buf_pointer_manages;

    long int frame_used;           //已使用张数
    long int frame_all;            //总张数
    long int buf_type;             //传入指针类型

    bool is_locked;
    uchar *buf_start;

public:
    long int width;                //图片宽
    long int height;               //图片长

};


#endif // GBUF_MANAGE_H
