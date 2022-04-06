# include"CH_algorithm.h"

//数组循环下标
int array_mod(int i,int mod)
{
    if (i>=0)
    {
        i=i%mod;
    }else
    {
        i=(i%mod)+mod;
    }
    return i;
}


/****************************int数组排序:start******************************/
//参考网址：https://blog.csdn.net/weixin_50886514/article/details/119045154?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522164506762716780264029095%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=164506762716780264029095&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~rank_v31_ecpm-5-119045154.pc_search_result_control_group&utm_term=%E4%B8%AD%E9%97%B4%E6%8E%92%E5%BA%8F&spm=1018.2226.3001.4187
//数组排序
//子函数声明
static void int_swap(int* a, int* b);//子函数声明
static int array_int_Select_sort(int* array,unsigned int n);//子函数声明
static int array_int_Bubble_sort(int* array,unsigned int n);
static int array_int_Insert_sort(int* array,unsigned int n);
static int array_int_Shell_sort(int* array,unsigned int n);
int array_int_sort(int* array,unsigned int n,int sequence)
{
    /*
    * 1.选择排序
    * 2.冒泡排序
    * 3.插入排序
    * 4.希尔排序（性能最好的排序）
    * 5.快速排序
    * 6.sort排序
    * 7.桶排序
    */

   array_int_Shell_sort(array,n);

    switch (sequence)
    {
    case 1://升序排序
        { return 0;}break;
    case -1://降序排序
        {
            for (unsigned int i = 0; i < n; i++)
            {   
                if(n%2==0)//n:偶数
                {
                    int_swap(array+i,array+(n-i-1));
                    if (i==((n/2)-1)){break;}
                }
                if(n%2==1)//n:奇数
                {
                    int_swap(array+i,array+(n-i-1));
                    if (i==(n+1)/2){ break;} 
                }
            }
            return 0;
        }break;
    default:
        return -1;
        break;
    }
}

//选择排序
static int array_int_Select_sort(int* array,unsigned int n)
{
    //保存参与单趟排序的第一个数和最后一个数的下标
	int begin = 0, end = n - 1;
	while (begin < end)
	{
		//保存最大值的下标
		int maxi = begin;
		//保存最小值的下标
		int mini = begin;
		//找出最大值和最小值的下标
		for (int i = begin; i <= end; ++i)
		{
			if (array[i] < array[mini])
			{
				mini = i;
			}
			if (array[i] > array[maxi])
			{
				maxi = i;
			}
		}
		//最小值放在序列开头
		int_swap(&array[mini], &array[begin]);
		//防止最大的数在begin位置被换走
		if (begin == maxi)
		{
			maxi = mini;
		}
		//最大值放在序列结尾
		int_swap(&array[maxi], &array[end]);
		++begin;
		--end;
	}
}

//冒泡排序
static int array_int_Bubble_sort(int* array,unsigned int n)
{
    int end = n;
	while (end)
	{
		int flag = 0;
		for (int i = 1; i < end; ++i)
		{
			if (array[i - 1] > array[i])
			{
				int tem = array[i];
				array[i] = array[i - 1];
				array[i - 1] = tem;
				flag = 1;
			}
		}
		if (flag == 0)
		{
			break;
		}
		--end;
	}
}

//插入排序
static int array_int_Insert_sort(int* array,unsigned int n)
{
    for (int i = 0; i < n - 1; ++i)
	{
		//记录有序序列最后一个元素的下标
		int end = i;
		//待插入的元素
		int tem = array[end + 1];
		//单趟排
		while (end >= 0)
		{
			//比插入的数大就向后移
			if (tem < array[end])
			{
				array[end + 1] = array[end];
				end--;
			}
			//比插入的数小，跳出循环
			else
			{
				break;
			}
		}
		//tem放到比插入的数小的数的后面
		array[end  + 1] = tem;
		//代码执行到此位置有两种情况:
		//1.待插入元素找到应插入位置（break跳出循环到此）
		//2.待插入元素比当前有序序列中的所有元素都小（while循环结束后到此）
	}
}

//希尔排序
static int array_int_Shell_sort(int* array,unsigned int n)
{
    int gap = n;
	while (gap>1)
	{
		//每次对gap折半操作
		gap = gap / 2;
		//单趟排序
		for (int i = 0; i < n - gap; ++i)
		{
			int end = i;
			int tem = array[end + gap];
			while (end >= 0)
			{
				if (tem < array[end])
				{
					array[end + gap] = array[end];
					end -= gap;
				}
				else
				{
					break;
				}
			}
			array[end + gap] = tem;
		}
	}
}

// int array_int_ _sort(int* array,unsigned int n)
// {
    
// }


static void int_swap(int* a, int* b)
{
	int tem = *a;
	*a = *b;
	*b = tem;
}
/******************************int数组排序:end******************************/



/******************************float数组排序:start******************************/
//参考网址：https://blog.csdn.net/weixin_50886514/article/details/119045154?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522164506762716780264029095%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=164506762716780264029095&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~rank_v31_ecpm-5-119045154.pc_search_result_control_group&utm_term=%E4%B8%AD%E9%97%B4%E6%8E%92%E5%BA%8F&spm=1018.2226.3001.4187
//数组排序
//子函数声明
static void float_swap(float* a, float* b);//子函数声明
static int array_float_Select_sort(float* array,unsigned int n);//子函数声明
static int array_float_Bubble_sort(float* array,unsigned int n);
static int array_float_Insert_sort(float* array,unsigned int n);
static int array_float_Shell_sort(float* array,unsigned int n);
int array_float_sort(float* array,unsigned int n,int sequence)
{
    /*
    * 1.选择排序
    * 2.冒泡排序
    * 3.插入排序
    * 4.希尔排序（性能最好的排序）
    * 5.快速排序
    * 6.sort排序
    * 7.桶排序
    */

   array_float_Insert_sort(array,n);

    switch (sequence)
    {
    case 1://升序排序
        { return 0;}break;
    case -1://降序排序
        {
            for (unsigned int i = 0; i < n; i++)
            {   
                if(n%2==0)//n:偶数
                {
                    float_swap(array+i,array+(n-i-1));
                    if (i==((n/2)-1)){break;}
                }
                if(n%2==1)//n:奇数
                {
                    float_swap(array+i,array+(n-i-1));
                    if (i==(n-1)/2){ break;} 
                }
            }
            return 0;
        }break;
    default:
        return -1;
        break;
    }
}

//选择排序
static int array_float_Select_sort(float* array,unsigned int n)
{
    //保存参与单趟排序的第一个数和最后一个数的下标
	int begin = 0, end = n - 1;
	while (begin < end)
	{
		//保存最大值的下标
		int maxi = begin;
		//保存最小值的下标
		int mini = begin;
		//找出最大值和最小值的下标
		for (int i = begin; i <= end; ++i)
		{
			if (array[i] < array[mini])
			{
				mini = i;
			}
			if (array[i] > array[maxi])
			{
				maxi = i;
			}
		}
		//最小值放在序列开头
		float_swap(&array[mini], &array[begin]);
		//防止最大的数在begin位置被换走
		if (begin == maxi)
		{
			maxi = mini;
		}
		//最大值放在序列结尾
		float_swap(&array[maxi], &array[end]);
		++begin;
		--end;
	}
}

//冒泡排序
static int array_float_Bubble_sort(float* array,unsigned int n)
{
    int end = n;
	while (end)
	{
		int flag = 0;
		for (int i = 1; i < end; ++i)
		{
			if (array[i - 1] > array[i])
			{
				float tem = array[i];
				array[i] = array[i - 1];
				array[i - 1] = tem;
				flag = 1;
			}
		}
		if (flag == 0)
		{
			break;
		}
		--end;
	}
}

//插入排序
static int array_float_Insert_sort(float* array,unsigned int n)
{
    for (int i = 0; i < n - 1; ++i)
	{
		//记录有序序列最后一个元素的下标
		int end = i;
		//待插入的元素
		float tem = array[end + 1];
		//单趟排
		while (end >= 0)
		{
			//比插入的数大就向后移
			if (tem < array[end])
			{
				array[end + 1] = array[end];
				end--;
			}
			//比插入的数小，跳出循环
			else
			{
				break;
			}
		}
		//tem放到比插入的数小的数的后面
		array[end  + 1] = tem;
		//代码执行到此位置有两种情况:
		//1.待插入元素找到应插入位置（break跳出循环到此）
		//2.待插入元素比当前有序序列中的所有元素都小（while循环结束后到此）
	}
}

//希尔排序
static int array_float_Shell_sort(float* array,unsigned int n)
{
    int gap = n;
	while (gap>1)
	{
		//每次对gap折半操作
		gap = gap / 2;
		//单趟排序
		for (int i = 0; i < n - gap; ++i)
		{
			int end = i;
			float tem = array[end + gap];
			while (end >= 0)
			{
				if (tem < array[end])
				{
					array[end + gap] = array[end];
					end -= gap;
				}
				else
				{
					break;
				}
			}
			array[end + gap] = tem;
		}
	}
}

static void float_swap(float* a, float* b)
{
	float tem = *a;
	*a = *b;
	*b = tem;
}
/******************************float数组排序:end******************************/