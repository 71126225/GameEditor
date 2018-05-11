#include "MathUtility.h"
#include "StringUtility.h"

float MathUtility::calculateFloat(std::string str)
{
	// �ж��ַ����Ƿ��зǷ��ַ�,Ҳ���ǳ�����,С����,�����������ַ�
	for (int i = 0; i < (int)str.length();)
	{
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.' && str[i] != '+'
			&& str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '(' && str[i] != ')')
		{
			str.erase(i, 1);
		}
		else
		{
			++i;
		}
	}
	// �ж��������������Ƿ����
	int leftBracketCount = 0;
	int rightBracketCount = 0;
	for (int i = 0; i < (int)str.length(); ++i)
	{
		if (str[i] == '(')
		{
			++leftBracketCount;
		}
		else if (str[i] == ')')
		{
			++rightBracketCount;
		}
	}
	if (leftBracketCount != rightBracketCount)
	{
		// �������,����������������Ӧ
		return 0;
	}

	// ѭ���жϴ�����ַ�����û������
	while (true)
	{
		// ���ж���û�����ţ���������ž������������,���û�о��˳�whileѭ��
		if (str.find_first_of("(") != -1 || str.find_first_of(")") != -1)
		{
			int curpos = str.find_last_of("(");
			std::string strInBracket = str.substr(curpos + 1, str.length() - curpos - 1);
			strInBracket = strInBracket.substr(0, strInBracket.find_first_of(")"));
			float ret = calculateFloat(strInBracket);
			// ��������еļ������Ǹ���,����Ϊ����
			bool isMinus = false;
			if (ret < 0)
			{
				ret = -ret;
				isMinus = true;
			}
			// �������еļ������滻ԭ���ı��ʽ,��������Ҳһ���滻
			std::string floatStr = StringUtility::floatToString(ret, 4);
			str = StringUtility::strReplace(str, curpos, curpos + strInBracket.length() + 2, floatStr);
			if (isMinus)
			{
				// ��������м�������Ǹ���,�򽫸�����ȡ����,����ߵĵ�һ���Ӽ��Ÿ�Ϊ�෴�ķ���
				bool changeMark = false;
				for (int i = curpos - 1; i >= 0; --i)
				{
					// �ҵ���һ��+��,��ֱ�Ӹ�Ϊ����,Ȼ���˳�����
					if (str[i] == '+')
					{
						str[i] = '-';
						changeMark = true;
						break;
					}
					// �ҵ���һ������,������ŵ����������,��ֱ�Ӹ�Ϊ+��
					// ������ŵ���߲�������,��ü����Ǹ���,������ȥ��,
					else if (str[i] == '-')
					{
						if (str[i - 1] >= '0' && str[i - 1] <= '9')
						{
							str[i] = '+';
						}
						else
						{
							str = StringUtility::strReplace(str, i, i + 1, "");
						}
						changeMark = true;
						break;
					}
				}
				// ����������˻�û���ҵ������滻�ķ���,���ڱ��ʽ��ǰ���һ������
				if (!changeMark)
				{
					str = "-" + str;
				}
			}
		}
		else
		{
			break;
		}
	}

	txVector<float> numbers;
	txVector<char> factors;
	// ��ʾ��һ����������±�+1
	int beginpos = 0;
	for (int i = 0; i < (int)str.length(); ++i)
	{
		// �����������һ���ַ�,��ֱ�Ӱ����һ�����ַ����б�,Ȼ���˳�ѭ��
		if (i == str.length() - 1)
		{
			std::string num = str.substr(beginpos, str.length() - beginpos);
			float fNum = StringUtility::stringToFloat(num);
			numbers.push_back(fNum);
			break;
		}
		// �ҵ���һ�������
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.')
		{
			if (i != 0)
			{
				std::string num = str.substr(beginpos, i);
				float fNum = StringUtility::stringToFloat(num);
				numbers.push_back(fNum);
			}
			// ����ڱ��ʽ�Ŀ�ʼ�ͷ����������,���ʾ��һ�����Ǹ���,�Ǿʹ���Ϊ0��ȥ������ľ���ֵ
			else
			{
				numbers.push_back(0);
			}
			factors.push_back(str[i]);
			beginpos = i + 1;
		}
	}
	if (factors.size() + 1 != numbers.size())
	{
		// �������,�������������������
		return 0;
	}
	// ���ڿ�ʼ������ʽ,�����������ȼ�,�ȼ���˳���ȡ��
	while (true)
	{
		// ��ʾ�Ƿ��г˳����ʽ
		bool hasMS = false;
		for (int i = 0; i < (int)factors.size(); ++i)
		{
			// �ȱ������ĸ����ȼ����ĸ�
			if (factors[i] == '*' || factors[i] == '/')
			{
				// ��һ�����������±�����������±�����ͬ��
				float num1 = numbers[i];
				float num2 = numbers[i + 1];
				float num3 = 0.0f;
				if (factors[i] == '*')
				{
					num3 = num1 * num2;
				}
				else if (factors[i] == '/')
				{
					num3 = num1 / num2;
				}
				// ɾ����i + 1����,Ȼ�󽫵�i�����滻Ϊ������
				numbers.erase(numbers.begin() + i + 1);
				if (numbers.size() == 0)
				{
					// �������
					return 0;
				}
				numbers[i] = num3;
				// ɾ����i�������
				factors.erase(factors.begin() + i);
				hasMS = true;
				break;
			}
		}
		if (!hasMS)
		{
			break;
		}
	}
	// �ټ���Ӽ���
	while (true)
	{
		if (factors.size() == 0)
		{
			break;
		}
		if (factors[0] == '+' || factors[0] == '-')
		{
			// ��һ�����������±�����������±�����ͬ��
			float num1 = numbers[0];
			float num2 = numbers[1];
			float num3 = 0.0f;
			if (factors[0] == '+')
			{
				num3 = num1 + num2;
			}
			else if (factors[0] == '-')
			{
				num3 = num1 - num2;
			}
			// ɾ����1����,Ȼ�󽫵�0�����滻Ϊ������
			numbers.erase(numbers.begin() + 1);
			if (numbers.size() == 0)
			{
				// �������
				return 0;
			}
			numbers[0] = num3;
			// ɾ����0�������
			factors.erase(factors.begin());
		}
	}
	if (numbers.size() != 1)
	{
		// �������
		return 0;
	}
	else
	{
		return numbers[0];
	}
}

int MathUtility::calculateInt(std::string str)
{
	// �ж��ַ����Ƿ��зǷ��ַ�,Ҳ���ǳ�����,С����,�����������ַ�
	for (int i = 0; i < (int)str.length();)
	{
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.' && str[i] != '+'
			&& str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '%'
			&& str[i] != '(' && str[i] != ')')
		{
			str.erase(i, 1);
		}
		else
		{
			++i;
		}
	}
	// �ж��������������Ƿ����
	int leftBracketCount = 0;
	int rightBracketCount = 0;
	for (int i = 0; i < (int)str.length(); ++i)
	{
		if (str[i] == '(')
		{
			++leftBracketCount;
		}
		else if (str[i] == ')')
		{
			++rightBracketCount;
		}
	}
	if (leftBracketCount != rightBracketCount)
	{
		// �������,����������������Ӧ
		return 0;
	}

	// ѭ���жϴ�����ַ�����û������
	while (true)
	{
		// ���ж���û�����ţ���������ž������������,���û�о��˳�whileѭ��
		if (str.find_first_of("(") != -1 || str.find_first_of(")") != -1)
		{
			int curpos = str.find_last_of("(");
			std::string strInBracket = str.substr(curpos + 1, str.length() - curpos - 1);
			strInBracket = strInBracket.substr(0, strInBracket.find_first_of(")"));
			int ret = calculateInt(strInBracket);
			// ��������еļ������Ǹ���,����Ϊ����
			bool isMinus = false;
			if (ret < 0)
			{
				ret = -ret;
				isMinus = true;
			}
			// �������еļ������滻ԭ���ı��ʽ,��������Ҳһ���滻
			std::string intStr = StringUtility::intToString(ret, 4);
			str = StringUtility::strReplace(str, curpos, curpos + strInBracket.length() + 2, intStr);
			if (isMinus)
			{
				// ��������м�������Ǹ���,�򽫸�����ȡ����,����ߵĵ�һ���Ӽ��Ÿ�Ϊ�෴�ķ���
				bool changeMark = false;
				for (int i = curpos - 1; i >= 0; --i)
				{
					// �ҵ���һ��+��,��ֱ�Ӹ�Ϊ����,Ȼ���˳�����
					if (str[i] == '+')
					{
						str[i] = '-';
						changeMark = true;
						break;
					}
					// �ҵ���һ������,������ŵ����������,��ֱ�Ӹ�Ϊ+��
					// ������ŵ���߲�������,��ü����Ǹ���,������ȥ��,
					else if (str[i] == '-')
					{
						if (str[i - 1] >= '0' && str[i - 1] <= '9')
						{
							str[i] = '+';
						}
						else
						{
							str = StringUtility::strReplace(str, i, i + 1, "");
						}
						changeMark = true;
						break;
					}
				}
				// ����������˻�û���ҵ������滻�ķ���,���ڱ��ʽ��ǰ���һ������
				if (!changeMark)
				{
					str = "-" + str;
				}
			}
		}
		else
		{
			break;
		}
	}

	txVector<int> numbers;
	txVector<char> factors;
	// ��ʾ��һ����������±�+1
	int beginpos = 0;
	for (int i = 0; i < (int)str.length(); ++i)
	{
		// �����������һ���ַ�,��ֱ�Ӱ����һ�����ַ����б�,Ȼ���˳�ѭ��
		if (i == str.length() - 1)
		{
			std::string num = str.substr(beginpos, str.length() - beginpos);
			int iNum = StringUtility::stringToInt(num);
			numbers.push_back(iNum);
			break;
		}
		// �ҵ���һ�������
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.')
		{
			if (i != 0)
			{
				std::string num = str.substr(beginpos, i);
				int iNum = StringUtility::stringToInt(num);
				numbers.push_back(iNum);
			}
			// ����ڱ��ʽ�Ŀ�ʼ�ͷ����������,���ʾ��һ�����Ǹ���,�Ǿʹ���Ϊ0��ȥ������ľ���ֵ
			else
			{
				numbers.push_back(0);
			}
			factors.push_back(str[i]);
			beginpos = i + 1;
		}
	}
	if (factors.size() + 1 != numbers.size())
	{
		// �������,�������������������
		return 0;
	}
	// ���ڿ�ʼ������ʽ,�����������ȼ�,�ȼ���˳���ȡ��
	while (true)
	{
		// ��ʾ�Ƿ��г˳����ʽ
		bool hasMS = false;
		for (int i = 0; i < (int)factors.size(); ++i)
		{
			// �ȱ������ĸ����ȼ����ĸ�
			if (factors[i] == '*' || factors[i] == '/' || factors[i] == '%')
			{
				// ��һ�����������±�����������±�����ͬ��
				int num1 = numbers[i];
				int num2 = numbers[i + 1];
				int num3 = 0;
				if (factors[i] == '*')
				{
					num3 = num1 * num2;
				}
				else if (factors[i] == '/')
				{
					num3 = num1 / num2;
				}
				else if (factors[i] == '%')
				{
					num3 = num1 % num2;
				}
				// ɾ����i + 1����,Ȼ�󽫵�i�����滻Ϊ������
				numbers.erase(numbers.begin() + i + 1);
				if (numbers.size() == 0)
				{
					// �������
					return 0;
				}
				numbers[i] = num3;
				// ɾ����i�������
				factors.erase(factors.begin() + i);
				hasMS = true;
				break;
			}
		}
		if (!hasMS)
		{
			break;
		}
	}
	// �ټ���Ӽ���
	while (true)
	{
		if (factors.size() == 0)
		{
			break;
		}
		if (factors[0] == '+' || factors[0] == '-')
		{
			// ��һ�����������±�����������±�����ͬ��
			int num1 = numbers[0];
			int num2 = numbers[1];
			int num3 = 0;
			if (factors[0] == '+')
			{
				num3 = num1 + num2;
			}
			else if (factors[0] == '-')
			{
				num3 = num1 - num2;
			}
			// ɾ����1����,Ȼ�󽫵�0�����滻Ϊ������
			numbers.erase(numbers.begin() + 1);
			if (numbers.size() == 0)
			{
				// �������
				return 0;
			}
			numbers[0] = num3;
			// ɾ����0�������
			factors.erase(factors.begin());
		}
	}
	if (numbers.size() != 1)
	{
		// �������
		return 0;
	}
	else
	{
		return numbers[0];
	}
}