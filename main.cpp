//软件工程 卓越班 连梓豪 202130490151
//date : 11.10
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<random>
#include<iomanip>
#include<ctime>
#include<queue>
#include<windows.h>
using namespace std;

//测试函数的序列 从0-6
#define FUNC_NUM 6

//测试序列的大小
const int numMax[]={100,1000,10000};
//测试文件的名称
const string fileName[]={"input100.txt","input1000.txt","input10000.txt"};
//存放测试序列
vector<vector<int>> arrays(3);

//判断测试序列是否有序
bool isSorted();
//从文件读取测试序列
void inputValue();
//对测试序列初始化
void init();
//打印测试序列
void printArrays();
//测试某个排序算法，返回运行时间
double testAlgorithm(vector<int>& array);
//对某个算法进行测试，计算三组序列的运行时间
void test();

//7组算法的声明
void InsertSort(vector<int>&array,int size);
void BubbleSort(vector<int>&array,int size);
void SelectionSort(vector<int>&array,int size);
void MergeSort(vector<int>&array,int size);
void QuickSort(vector<int>&array,int size);
void HeapSort(vector<int>&array,int size);
void RadixSort(vector<int>&array,int size);

//MergeSort需要的辅助函数
void mergeSort(vector<int>&array,int left,int right);
void merge(vector<int>&array,int left,int mid,int right);

//QuickSort需要的辅助函数
void quickSort(vector<int>&array,int left,int right);

//RadixSort需要的辅助函数
int maxBit(vector<int>&array);

//各组算法的函数指针
void (*pSortFunc[7])(vector<int>&,int)={InsertSort,BubbleSort,SelectionSort,MergeSort,
                                QuickSort,HeapSort,RadixSort};
//各组算法的名称
const string sortFuncName[7]={"InsertSort","BubbleSort","SelectionSort","MergeSort",
                                "QuickSort","HeapSort","RadixSort"};

int main()
{
    //初始化测试序列
    init();
    //判断是否有序
    cout<<"before sort "<<"sorted? "<<boolalpha<<isSorted()<<endl;
    //算法进行排序
    test();
    //判断是否有序
    cout<<"After sort "<<"sorted? "<<boolalpha<<isSorted()<<endl;
    system("pause");
}

bool isSorted()
{
    //判断所有的测试数据是否升序
    for(int i=0;i<3;++i)
    {
        for(int j=1;j<numMax[i];++j)
        {
            if(arrays[i][j]<arrays[i][j-1])
                return false;
        }
    }
    return true;
}

void init()
{
    //根据测试数据数量对数组初始化
    for(int i=0;i<3;++i)
    {
        arrays[i].resize(numMax[i]);
    }
    inputValue();
}

void inputValue()
{
    //打开测试文件，读取测试数据
    fstream file;
    for(int i=0;i<3;++i)
    {
        file.open(fileName[i],ios::in|ios::out);
        if(!file)
            exit(-1);
        for(int j=0;j<numMax[i];++j)
        {
            int temp;
            file>>temp;
            arrays[i][j]=temp;
        }
        file.close();
    }
}

void printArrays()
{
    //输出三组测试序列
    for(auto array:arrays)
    {
        for(auto item:array)
            cout<<item<<" ";
        cout<<endl;
    }
}

double testAlgorithm(vector<int>& array)
{
    //用于计时间
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	QueryPerformanceFrequency(&nFreq);
    //计时开始
	QueryPerformanceCounter(&nBeginTime);
    //算法对array排序
    (*pSortFunc[FUNC_NUM])(array,array.size());
    //计时结束
	QueryPerformanceCounter(&nEndTime);  
    //返回运行时间
	return  1000*(double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
}
void test()
{
    //提示算法名称
    cout<<"\nuse "<<sortFuncName[FUNC_NUM]<<" algorithm to sort\n\n";
    for(int i=0;i<3;++i)
    {
        //测量算法运行时间
        double time=testAlgorithm(arrays[i]);
        //输出
        cout<<"sort "<<setw(7)<<left<<numMax[i]<<" cost "<<time<<"ms"<<endl;
    }
    cout<<endl;
}

void InsertSort(vector<int>&array,int size)
{
    //0-right表示有序
    for(int right=0;right<size;++right)
    {
        //right是要插入的元素
        int insert=array[right];
        int insertPosition=right;
        //寻找可以插入的位置
        for(int left=0;left<right;++left)
        {
            if(array[left]<insert)
                continue;
            insertPosition=left;
            break;
        }
        //将插入位置以后的元素后移
        for(int start=right;start>insertPosition;--start)
        {
            array[start]=array[start-1];
        }
        //插入
        array[insertPosition]=insert;
    }
}

void BubbleSort(vector<int>&array,int size)
{
    //0-right表示冒泡的区域
    for(int right=size-1;right>0;right--)
    {
        //从前往后，将最大的元素后移
        for(int left=0;left<right;++left)
        {
            if(array[left]>array[left+1])
            {
                swap(array[left],array[left+1]);
            }
        }
    }
}

void SelectionSort(vector<int>&array,int size)
{
    //right以后的区域有序
    for(int right=size-1;right>0;--right)
    {
        //寻找无序区域的最大值
        int max=0;
        for(int i=1;i<=right;++i)
        {
            if(array[max]<array[i])
                max=i;
        }
        //将最大值和无序区边界值交换
        swap(array[max],array[right]);
    }
}

void MergeSort(vector<int>&array,int size)
{
    //调用辅助函数，从0-size-1进行归并排序
    mergeSort(array,0,size-1);
}

void mergeSort(vector<int>&array,int left,int right)
{
    //已经满足 返回
    if(left>=right)
        return;
    //计算中值
    int mid=(left+right)/2;
    //对前面归并排序
    mergeSort(array,left,mid);
    //对后面归并排序
    mergeSort(array,mid+1,right);
    //将两部分归并好的有序区域合并
    merge(array,left,mid,right);
}

void merge(vector<int>&array,int left,int mid,int right)
{
    //构建两个辅助的left和right数组
    vector<int>leftArray(array.begin()+left,array.begin()+mid+1);
    vector<int>rightArray(array.begin()+mid+1,array.begin()+right+1);
    //分别放入两个最大值，可以减少代码复杂度
    leftArray.push_back(INT_MAX);
    rightArray.push_back(INT_MAX);
    int lp=0;
    int rp=0;
    //从头到尾插入最小值
    for(int i=left;i<=right;++i)
    {
        if(leftArray[lp]>rightArray[rp])
            array[i]=rightArray[rp++];
        else
            array[i]=leftArray[lp++];
    }
}

void QuickSort(vector<int>&array,int size)
{
    //调用辅助函数快速排序
    quickSort(array,0,size-1);
}

void quickSort(vector<int>&array,int left,int right)
{
    //已经满足，返回
    if(left>=right)
        return;
    //选择i作为基数的位置
    int i=left;
    int j=right;
    //x保存基数
    int x=array[left];
    //当i<j继续
    while(i<j)
    {
        //如果右边的数据都大于基数，就--j
        while(i<j&&array[j]>=x)
            j--;
        //右边存在一个数小于基数，换到左边
        if(i<j)
            array[i++]=array[j];
        //如果左边数小于基数，++i
        while(i<j&&array[i]<x)
            i++;
        //左边存在一个数大于基数，换到右边
        if(i<j)
            array[j--]=array[i];
    }
    //将空洞换成基数
    array[i]=x;
    //对左右两区域进行快速排序
    quickSort(array,left,i-1);
    quickSort(array,i+1,right);
}

void HeapSort(vector<int>&array,int size)
{
    //用输入序列构建一个大顶堆
    priority_queue<int> heap(array.begin(),array.end());
    //不断地删除堆的根节点来排序
    for(int i=size-1;i>=0;--i)
    {
        array[i]=heap.top();
        heap.pop();
    }
}

void RadixSort(vector<int>&array,int size)
{
    //获取整个数组中的最大位数
    int n=maxBit(array);
    //获取位数的辅助数据 
    int dividend=1;
    //设置10个桶
    vector<vector<int>> buckets(10);
    //遍历n次
    for(int i=0;i<n;++i)
    {
        //将所有数据放入对应桶中
        for(int j=0;j<size;++j)
        {
            //根据当前选择的位数来放入insert位置的bucket中
            int insert=(array[j]%(10*dividend))/dividend;
            buckets[insert].push_back(array[j]);
        }
        int j=0;
        //遍历所有的桶，将数据填入表中
        for(auto& bucket:buckets)
        {
            for(auto value:bucket)
            {
                array[j++]=value;
            }
            //将桶复原
            bucket.resize(0);
        }
        //遍历下一位
        dividend*=10;
    }
}

int maxBit(vector<int>&array)
{
    //先寻找最大的数据
    int maxValue=0;
    for(auto value:array)
        if(maxValue<value)
            maxValue=value;
    int result=1;
    //判断最大数据的位数
    while(maxValue>=10)
    {
        ++result;
        maxValue/=10;
    }
    return result;
}