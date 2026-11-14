#include "gbuf_manage.h"
#include "parammanage.h"



Memory_Manage::Memory_Manage(uchar * start,long int size_)
{
    Create(start,size_);
}

bool Memory_Manage::Create(uchar* start,long int size_)
{
    start_ptr=start;
    size=size_;
    now_loca=0;

    return true;
}

bool Memory_Manage::new_buf_(uchar** ptr,long int size_)
{
    long int memory_rest=size-now_loca;
    if(size_<=memory_rest)
    {
        *ptr=start_ptr+now_loca;

        now_loca+=size_;
        std::pair<uchar **,int> new_ptr(ptr,size_);
        manages.push(new_ptr);
    }
    else
        return false;

    return true;
}


bool Memory_Manage::free_buf_(uchar ** ptr)
{
    std::pair<uchar **,int>  ptrs=manages.top();
    uchar** top_ptr=ptrs.first;
    if(ptr!=top_ptr)
        return false;
    else
    {
        long int loca_res=now_loca-ptrs.second;
        if(manages.size()>0 && loca_res>=0)
        {
//            *ptr=nullptr;
            now_loca=loca_res;
            manages.pop();
        }
        else
            return false;
    }
    return true;
}


Pointer_status::Pointer_status(Memory_Area status_, Buf_type Buftype_size_,int frame_cnt_):
    memory_status(status_),Buftype(Buftype_size_),frame_cnt(frame_cnt_)
{

}



Gbuf_Manage::Gbuf_Manage(uchar *ptr, int width_, int height_,
                         int frame_all_, int resultcnt_, int buf_type_)
{
    Create(ptr,width_,height_,frame_all_, resultcnt_,buf_type_);
}

bool Gbuf_Manage::Create(uchar *ptr, int width_, int height_,
                        int frame_all_, int resultcnt_, int buf_type_)
{
    buf_start=ptr;
    width=width_;
    height=height_;

    frame_used=frame_all_-resultcnt_;
    buf_type=buf_type_;
    is_locked=true;

    long int skip_buf= 2*width*height*(buf_type/8);                      //跳过起始2张图
    long int used_buf_size =(frame_used-2)*width*height*(buf_type/8);    //计算区使用6张图
    long int res_buf_size = resultcnt_*width*height*(buf_type/8);        //结果区使用4张图

    g_buf_alculate_s.Create(buf_start+skip_buf,used_buf_size);
    g_buf_result_s.Create(buf_start+skip_buf+used_buf_size,res_buf_size);

    //尝试访问最后一张最后一个像素
    logMsg ("try change last png!",0);
    long int last_img_start_loca=(frame_all_)*width*height*(buf_type/8)-1;
    ptr[last_img_start_loca]=0;
    logMsg ("change last png succeed!",0);

    return true;
}


bool Gbuf_Manage::new_buf_(uchar** ptr,Memory_Area Area,Buf_type type,
                           int width_,int height_,int famer_cnt)
{
    bool res=false;
    long int buf_size= famer_cnt* width_* height_* type/8;

    if(Area==Result_area)
        res= g_buf_result_s.new_buf_(ptr,buf_size);
    else if(Area == Alculate_area
            && !is_locked)
        res= g_buf_alculate_s.new_buf_(ptr,buf_size);

    if(res)
    {
        Pointer_status status(Area,type,famer_cnt);
        buf_pointer_manages[ptr]=status;
    }
    return res;
}


bool Gbuf_Manage::free_buf_(uchar **ptr)
{
    if(buf_pointer_manages.find(ptr)!=buf_pointer_manages.end())
    {
        Pointer_status status=buf_pointer_manages[ptr];

        bool res=false;
        if(status.show_memory_status()==Result_area)
            res= g_buf_result_s.free_buf_(ptr);
        else if(status.show_memory_status()==Alculate_area)
            res= g_buf_alculate_s.free_buf_(ptr);
        if(res)
        {
            buf_pointer_manages.erase(ptr);
            return true;
        }
    }
    return false;
}


bool Gbuf_Manage::Free_alculate_buf()
{
    uchar **top_p=g_buf_alculate_s.top_pointer();
    while(top_p!=NULL)
    {
        if(!free_buf_(top_p))                //注  不能将*top_p=nullptr，否则会导致访问未知内存（当该指针 *top_p  被系统删除后，仍未free_buf_）
            return false;

        top_p=g_buf_alculate_s.top_pointer();
    }
    return true;
}

bool Gbuf_Manage::Free_result_buf()
{
    uchar **top_p=g_buf_result_s.top_pointer();
    while(top_p!=NULL)
    {
        if(!free_buf_(top_p))
            return false;
        top_p=g_buf_result_s.top_pointer();
    }
    return true;
}


bool Gbuf_Manage::Free_all_buf()
{
    bool flag=true;
    flag= Free_alculate_buf();
    flag=Free_result_buf();

    return flag;
}

