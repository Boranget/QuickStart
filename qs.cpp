#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <Windows.h>
using namespace std;

/**
 * 切割一个字符串
 * 传入原字符串,切割字符,用于接收的字符串数组
 */
void split(string src, string delim, string *res)
{
	// 由于strtok会更改字符串本身,但传入的参数是只读的,所以这里拷贝一份
	int len = src.length();
	char cache[len];
	strcpy(cache, src.c_str());
	char *node;
	res[0] = strtok(cache, delim.c_str());
	for (int i = 1; node = strtok(NULL, delim.c_str()); i++)
	{
		res[i] = (string)node;
	}
	return;
}
/**
 * 初始化
 *
 */
vector<string *> init()
{
	// 获取环境变量中QS_HOME的值
	char buffer[MAX_PATH];
	GetEnvironmentVariableA("QS_HOME", buffer, MAX_PATH);
	// 组合为文件名
	string fileName = (string)buffer+"\\"+"qsConfig";
	vector<string *> res;
	// 读文件准备
	// 文件读取对象
	ifstream readFile;
	// 打开文件
	readFile.open(fileName, ios::in);
	string currentLine;
	while (!readFile.eof())
	{
		readFile >> currentLine;
		// 若每一行结尾为换行符,去掉
		if (currentLine[currentLine.length() - 1] == '\n')
		{
			currentLine[currentLine.length() - 1] == '\0';
		}
		// 切割该行字符串,获得切割结果
		string *currentLineArry = new string[3];
		split(currentLine, ",", currentLineArry);
		res.push_back(currentLineArry);
	}
	return res;
}
/**
 * 执行路径
 */
int executeQs(string path)
{
	string pathf = "\nstart \" \" ";
	// 连接
	string commondStr = pathf + path;
	// 输出命令
	cout << commondStr << endl;
	// 执行命令,c_str()将字符串转为字符数组
	system(commondStr.c_str());
	return 0;
}

//参数为空的情况下
//输入id，
//通过id获得path，然后执行
int showConfigList(vector<string *> form)
{

	string path;
	// 输出表格头
	printf("%-10s%-15s%-15s%-50s\n\n", "ID", "Name", "Parameter", "Path");
	// 遍历表格并输出
	for (int i = 0; i < form.size(); i++)
	{
		printf("%-10d%-15s%-15s%-50s", i + 1, form[i][0].c_str(), form[i][1].c_str(), form[i][2].c_str());
		cout << endl;
	}

	// 用户输入启动命令编号
	int id;
	cout << "Please enter the ID number : ";
	cin >> id;
	cout << endl;

	if (id < 1 || id > form.size())
	{
		cerr << "Id number is not exist" << endl;
		return 0;
	}
	cout << "Please wait a moment ...... " << endl;
	path = form[id - 1][2];
	executeQs(path);

	return 1;
}

//参数不为空的情况下
//传入参数，通过参数查找id，
//通过id获得path，然后执行
int serchByQuickName(string parameter, vector<string *> form)
{
	string path;
	int i = 0;
	for (i = 0; i < form.size(); i++)
	{
		if (!parameter.compare(form[i][1]))
		{
			path = form[i][2];
			break;
		}
	}
	if (i == form.size())
	{
		cerr << "Parameter is not exist" << endl;
		return 0;
	}

	cout << "Please wait a moment ...... " << endl;
	executeQs(path);

	return 1;
}

int main(int argc, char *argv[])
{
	// 初始化
	vector<string *> form = init();
	if (argc == 2)
	{
		// 如果命令行参数长度为2,说明带上了启动命令
		// 获取启动命令参数
		char *parameter = argv[1];
		// 根据命令查找
		serchByQuickName(parameter, form);
	}
	else if (argc == 1)
	{
		// 如果长度为1, 说明没有带参数,展示列表
		showConfigList(form);
	}
	else
	{
		// 否则报错
		cerr << "Parameter is Error" << endl;
	}
	return 0;
}
