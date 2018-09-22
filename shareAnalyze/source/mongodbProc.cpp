// mongodb
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/document/value.hpp>
#include <bsoncxx/document/view.hpp>
#include <bsoncxx/array/view.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/stdx/string_view.hpp>
#include <bsoncxx/string/to_string.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/types/value.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/find.hpp>
#include <mongocxx/uri.hpp>

using namespace bsoncxx;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;
using bsoncxx::to_json;


/*************************************************************************************************
* 将UTF8编码转换成Unicode（UCS-2LE）编码  低地址存低位字节
* 参数：
*    char* pInput     输入字符串
*    char*pOutput   输出字符串
* 返回值：转换后的Unicode字符串的字节数，如果出错则返回-1
**************************************************************************************************/
//utf8转unicode
int Utf8ToUnicode(const char* pInput, char* pOutput)
{
	int outputSize = 0; //记录转换后的Unicode字符串的字节数

	while (*pInput)
	{
		if (*pInput > 0x00 && *pInput <= 0x7F) //处理单字节UTF8字符（英文字母、数字）
		{
			*pOutput = *pInput;
			//pOutput++;
			//*pOutput = 0; //小端法表示，在高地址填补0
		}
		else if (((*pInput) & 0xE0) == 0xC0) //处理双字节UTF8字符
		{
			char high = *pInput;
			pInput++;
			char low = *pInput;
			if ((low & 0xC0) != 0x80)  //检查是否为合法的UTF8字符表示
			{
				return -1; //如果不是则报错
			}

			*pOutput = (high << 6) + (low & 0x3F);
			pOutput++;
			*pOutput = (high >> 2) & 0x07;
		}
		else if (((*pInput) & 0xF0) == 0xE0) //处理三字节UTF8字符
		{
			char high = *pInput;
			pInput++;
			char middle = *pInput;
			pInput++;
			char low = *pInput;
			if (((middle & 0xC0) != 0x80) || ((low & 0xC0) != 0x80))
			{
				return -1;
			}
			*pOutput = (high << 4) + ((middle >> 2) & 0x0F); //取出high的低四位与middle的中间四位，组合成unicode字符的高8位
			pOutput++;
			*pOutput = (middle << 6) + (low & 0x3F);//取出middle的低两位与low的低6位，组合成unicode字符的低8位
		}
		else //对于其他字节数的UTF8字符不进行处理
		{
			return -1;
		}
		pInput++;//处理下一个utf8字符
		pOutput++;
		outputSize += 2;
	}
	//unicode字符串后面，有两个\0
	*pOutput = 0;
	pOutput++;
	*pOutput = 0;
	return outputSize;
}
/*std::string UnicodeToAscii(const wstring& wstr)
{
		// 预算-缓冲区中多字节的长度    
		int ansiiLen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
		// 给指向缓冲区的指针变量分配内存    
		char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);
		// 开始向缓冲区转换字节    
		WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);

		std::string ret_str = pAssii;
		free(pAssii);
		return ret_str;
}*/

void mongodbDataRead()
{
    // Lets first build up a non-trivial BSON document using the builder interface.
	using builder::basic::kvp;
	using builder::basic::sub_array;

	mongocxx::instance instance{}; // This should be done only once.
	//mongocxx::uri uri("mongodb://localhost:27017");
	//mongocxx::client client(uri);
	mongocxx::client client{ mongocxx::uri{} };

	mongocxx::database db = client["WenCai"];
	mongocxx::collection coll = db["wencai_20180817"];
	auto cursor = coll.find({});
	auto docCount = std::distance(cursor.begin(), cursor.end());
	cursor = coll.find({});
	int docIndex = 1;


	vector<std::string> wenCaiCodeVec;
	vector<float> wenCaiHuanShouVec;
	vector<float> wenCaiChengJiaoLiangVec;
	for (auto &&doc : cursor) 
	{
		if (docIndex < docCount)
		{
			++docIndex;
			continue;
		}
		else
		{
			// document::value is an owning bson document conceptually similar to string.
			document::value value{ doc };
			// document::view is a non-owning bson document conceptually similar to string_view.
			document::view view{ value.view() };
			int minLen = 0;
			// iterate over the elements in a bson document
			for (document::element ele : view) {
				// element is non owning view of a key-value pair within a document.

				// we can use the key() method to get a string_view of the key.
				stdx::string_view field_key{ ele.key() };

				std::cout << "Got key, key = " << field_key << std::endl;
				
				// we can use type() to get the type of the value.
				switch (ele.type())
				{
				case type::k_int32:
					minLen = ele.get_int32().value;
					break;
				case type::k_utf8:
					std::cout << "Got String!" << std::endl;
					break;
				case type::k_oid:
					std::cout << "Got ObjectId!" << std::endl;
					break;
				case type::k_array:
				{
					std::cout << "Got Array!" << std::endl;
					std::string fieldKey = field_key.to_string();
					char keyStr[100];
					char temp[20] = "dde大单净量";
					int flag = Utf8ToUnicode(fieldKey.c_str(), keyStr);
					// if we have a subarray, we can access it by getting a view of it.
					bsoncxx::array::view subarr{ ele.get_array().value };
					if (strcmp(field_key.data(), "code") == 0)
					{
						wenCaiCodeVec.reserve(minLen);
						for (bsoncxx::array::element ele : subarr)
						{
							std::string strCode = bsoncxx::string::to_string(ele.get_utf8().value);
							wenCaiCodeVec.push_back(strCode);
							//std::cout << "array element: " << bsoncxx::string::to_string(ele.get_utf8().value) << std::endl;
						}
					}
					if (strcmp(field_key.data(), "换手率") == 0)
					{
						wenCaiHuanShouVec.reserve(minLen);
						for (bsoncxx::array::element ele : subarr)
						{
							std::string str = bsoncxx::string::to_string(ele.get_utf8().value);
							float huanshou = atof(str.c_str());
							wenCaiHuanShouVec.push_back(huanshou);
							//std::cout << "array element: " << bsoncxx::string::to_string(ele.get_utf8().value) << std::endl;
						}
					}
					if (strcmp(field_key.data(), "成交量") == 0)
					{
						wenCaiChengJiaoLiangVec.reserve(minLen);
						for (bsoncxx::array::element ele : subarr)
						{
							std::string str = bsoncxx::string::to_string(ele.get_utf8().value);
							float chengjiao = atof(str.c_str());
							wenCaiChengJiaoLiangVec.push_back(chengjiao);
							//std::cout << "array element: " << bsoncxx::string::to_string(ele.get_utf8().value) << std::endl;
						}
					}
					break;
				}
				default:
					std::cout << "We messed up!" << std::endl;
				}

				// usually we don't need to actually use a switch statement, because we can also
				// get a variant 'value' that can hold any BSON type.
				types::value ele_val{ ele.get_value() };
			}
		}
	}

	/*bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
		coll.find_one(document{} << "minLen" << 3419 << finalize);
	if (maybe_result) {
		// Do something with *maybe_result;
		std::cout << maybe_result.view() << "\n";
	}*/

}

