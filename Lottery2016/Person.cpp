#include "pch.h"
#include "Person.h"
#include "Resources\resource.h"
#include "Items.h"
#include <filesystem>
#include <fstream>
#include <algorithm>

using namespace std;
using namespace std::experimental::filesystem;

bool inited = false;
static unordered_map<int, vector<int>> luckIds;

vector<int> GetLuckyPersonIds(int itemId)
{
	if (itemId >= 0 && itemId < (int)GetItems().size())
	{
		string filename = to_string(itemId) + ".txt";
		if (exists(path{ filename }))
		{
			fstream fs{ filename };
			vector<int> result;
			while (true)
			{
				int id;
				fs >> id;
				if (fs.eof())
				{
					break;
				}
				result.push_back(id);
			}
			return move(result);
		}
	}
	return vector<int>{};
}

std::vector<Person> PersonIdsToPersons(const std::vector<int>& workIds)
{
	vector<Person> result;
	auto & all = GetAllPerson();
	for (auto i : workIds)
	{
		result.push_back(all[i]);
	}
	return result;
}


void DeleteLuckyPersons()
{
	for (size_t i = 0; i < GetItems().size(); ++i)
	{
		string filename = to_string(i) + ".txt";
		remove(filename);
	}
}

CString CreateLuckyStatusFile(int itemId, std::vector<int>& personIds)
{
	locale loc("chs");
	wfstream fs{ "temp.txt", ios::trunc | ios::out};
	fs.imbue(loc);

	auto persons = PersonIdsToPersons(personIds);
	fs << GetItems()[itemId].Name << L" 的获奖名单如下：" << endl;
	for (auto& person : persons)
	{
		fs << L"姓名: " << person.Name << L"\t" << L"工号: " << person.WorkId << endl;
	}
	fs << "=========";
	return L"temp.txt";
}

const void SaveLuckyPersonIds(int itemId, const unordered_set<int>& personWorkIds)
{
	vector<int> existing = GetLuckyPersonIds(itemId);

	string filename = to_string(itemId) + ".txt";
	fstream fs;
	fs.open(filename, ios::out | ios::app);
	for (auto& p : personWorkIds)
	{
		if (find(begin(existing), end(existing), p) == existing.end())
		{
			fs << p << " ";
		}
	}
}

std::vector<Person> GetUnluckyPerson()
{
	auto ids = GetUnluckyPersonIds();
	return PersonIdsToPersons(ids);
}

std::vector<int> GetUnluckyPersonIds()
{
	vector<int> allPersonIds(GetAllPerson().size());

	for (size_t i = 0; i < GetItems().size(); ++i)
	{
		auto luckyIds = GetLuckyPersonIds(i);
		for (auto id : luckyIds)
		{
			allPersonIds[id] = -1;
		}
	}

	vector<int> personIds;
	for (size_t i = 0; i < allPersonIds.size(); ++i)
	{
		if (allPersonIds[i] != -1) personIds.push_back(i);
	}
	return personIds;
}

Person::Person(const int id, const wchar_t * name, const UINT resourceId, const wchar_t* notes) :
	WorkId(id),
	Name(name),
	ResourceId(resourceId),
	Notes(notes)
{
}


const vector<Person>& GetAllPerson()
{
	static vector<Person> result =
	{
		{ 1294781813,L"3c4c049d", IDR_PERSON1, L"4298aace 6da0 4108 a1dc 39065776bf64" },
		{ 1214049770,L"528fe714", IDR_PERSON2, L"195d5ee3 0796 4bd4 ab92 88954ffe4ed5" },
		{ 562998718,L"f84e77cd", IDR_PERSON3, L"1f574200 1576 49e0 b027 efd1fcde1f89" },
		{ 1393754471,L"32ca56d5", IDR_PERSON4, L"0bdf027f a388 485e b1ee 046418d03a30" },
		{ 1397647132,L"2ca53ef3", IDR_PERSON5, L"3bb83c4f acbd 4086 a60b 450e166d065f" },
		{ 1991451919,L"99175969", IDR_PERSON6, L"25bb5552 fec3 4964 b83f f97c749b8f4f" },
		{ 222847566,L"5407261f", IDR_PERSON7, L"5e79c0a7 99e3 4e20 ab33 972001f8839f" },
		{ 1944620337,L"1ec20be0", IDR_PERSON8, L"987bb1ae 18a7 4927 b613 4d24907b868b" },
		{ 1507969144,L"8fe1a106", IDR_PERSON9, L"86350cc4 268d 49f4 9701 e0310a4a9f5f" },
		{ 202992845,L"16fffc1b", IDR_PERSON10, L"2825a193 e237 4ce4 b64f ec3a0d4588d0" },
		{ 1285195404,L"6dece94b", IDR_PERSON11, L"38bb0de7 785b 4906 9344 1dd0e318169a" },
		{ 1184908955,L"f19a6ab2", IDR_PERSON12, L"aa77ef8f ac33 4d24 9580 f29d79177dfd" },
		{ 1535631824,L"18dbba36", IDR_PERSON13, L"be99e92d 35e7 4954 9ecc f1704e41aa9b" },
		{ 1575878276,L"f677351f", IDR_PERSON14, L"eddc2bab c656 4b06 ba97 e369b81b3031" },
		{ 1526305923,L"efc5a247", IDR_PERSON15, L"f8be6fa8 96bc 4147 b39a 0f7955be382c" },
		{ 485704790,L"f5c77123", IDR_PERSON16, L"695d5691 ed0d 4b76 a057 1c485cb314f7" },
		{ 194068894,L"882854cd", IDR_PERSON17, L"d1009637 943d 43b2 978b 2ecfc0f7df40" },
		{ 1755921976,L"ac97c03d", IDR_PERSON18, L"b851a643 17a9 41a8 a9a4 bbb586116be9" },
		{ 865092681,L"98b7f9ec", IDR_PERSON19, L"6da9f211 0735 421d 9b97 4c17f2fd5ef6" },
		{ 114295603,L"8fa37dd6", IDR_PERSON20, L"dc57d683 7bc6 4e92 abc0 afc897b89a34" },
		{ 429810217,L"2350f696", IDR_PERSON21, L"1611f7e9 11b7 4b48 9763 8cd6df679693" },
		{ 1632947076,L"22c0baf5", IDR_PERSON22, L"3840d54a a5a7 4a77 a845 86c84f2b1bb5" },
		{ 1405660052,L"93266c6d", IDR_PERSON23, L"f6d5d30f 22cb 4fbf bfa5 c6255a088b00" },
		{ 1790844998,L"3e7612f1", IDR_PERSON24, L"9c74de95 2bad 4289 812e 679131d11168" },
		{ 921343995,L"ca6d15dd", IDR_PERSON25, L"da0cd762 8f7a 4788 abd1 236a35f25f66" },
		{ 1592545307,L"0cc3467c", IDR_PERSON26, L"c6dab8d5 d197 4129 95c5 6ce45034ecd4" },
		{ 1434400452,L"8f68e599", IDR_PERSON27, L"241edef8 9163 4006 a4a0 724fb2ef5945" },
		{ 716676410,L"d1ccf8da", IDR_PERSON28, L"5f0cecce fd02 4c5d bc54 e9de39b96008" },
		{ 1544327199,L"54b6d0d4", IDR_PERSON29, L"729313c2 bde6 4fd7 afce df037e33bc71" },
		{ 535614167,L"f4d4ba66", IDR_PERSON30, L"dbf1491f 19f5 4a30 a57f c61c14dc77bc" },
		{ 73650696,L"2d4f176b", IDR_PERSON31, L"cb0cd0be 7f13 48ec 9761 bb9533177e91" },
		{ 572091480,L"a9c9d2f3", IDR_PERSON32, L"89a61972 6cdd 46f0 a1d5 ce56cc4c9320" },
		{ 205497932,L"e55c3ad1", IDR_PERSON33, L"8a27c7b6 f0ab 4e72 adaa 54b01aa82252" },
		{ 1048624660,L"4f2cbf74", IDR_PERSON34, L"9c2fee3b 487b 4e9e 94f4 d0c253dd3d36" },
		{ 626334222,L"75a8eb52", IDR_PERSON35, L"8667bfd8 2ae9 4dca 9ab9 b378ea30e791" },
		{ 935694631,L"77b080ce", IDR_PERSON36, L"1ae2136b ebb0 465d 8313 65c94b6e7f35" },
		{ 986813988,L"35517c5b", IDR_PERSON37, L"1aac6c2a 7c2c 44d4 84bc 3bb3de995424" },
		{ 2007705597,L"69775982", IDR_PERSON38, L"f2dc2660 16ee 47d5 a640 c8ba9c3d90ff" },
		{ 1992006767,L"f2a72469", IDR_PERSON39, L"43da9b03 5a81 4e7c bbc7 2816f8048e15" },
		{ 643954443,L"2b8b5b78", IDR_PERSON40, L"933414ae 5632 4669 b8f1 c19e1eb227f1" },
		{ 2007962110,L"866b2d54", IDR_PERSON41, L"ebc64037 db80 4076 9059 4b5585899712" },
		{ 960845043,L"669df16d", IDR_PERSON42, L"165ed999 3944 409d a7eb e7b2bc91e249" },
		{ 952644365,L"4997abff", IDR_PERSON43, L"541e422f 79fb 4827 8bed 92a8c9ba5c24" },
		{ 220377128,L"57e0d1c0", IDR_PERSON44, L"453d8b34 2e70 4321 9ea7 ea9b5be3fbc9" },
		{ 656382536,L"b3986a02", IDR_PERSON45, L"dcbbaa47 2448 42e2 803b fc2d610f9fd3" },
		{ 552155160,L"d449cbde", IDR_PERSON46, L"23dd4345 1c14 42bd 9881 09f66766712c" },
		{ 1821730797,L"3d377fcf", IDR_PERSON47, L"433f615c 37ff 468b aa6d 4c39fe2b1a3e" },
		{ 777007448,L"7f7e9085", IDR_PERSON48, L"58922a03 9f49 4544 a90d cf1fc7c8827c" },
		{ 738884778,L"e02115b2", IDR_PERSON49, L"fe67421b c44d 4acb ac13 e414654243fa" },
		{ 943953695,L"9471e0b9", IDR_PERSON50, L"d58d3090 7961 4740 a71b 5573eb038a9f" },
		{ 548304174,L"078167b3", IDR_PERSON51, L"4c25bfe1 fab9 4d32 8cb7 f5e477cf89ad" },
		{ 604620240,L"50611dfd", IDR_PERSON52, L"32987e64 4b20 407a 8c4a 98ea6fd27c78" },
		{ 1956729770,L"faf90366", IDR_PERSON53, L"257b870e c69c 46a1 985b 82342fc5f09c" },
		{ 90739540,L"02699782", IDR_PERSON54, L"1783afaf 2b40 4c5f 8fc4 d9406b825a7b" },
		{ 942436133,L"73fe53e4", IDR_PERSON55, L"9245d8ba 003a 4e74 8c3a 9392a0125502" },
		{ 1809318384,L"3c0679b2", IDR_PERSON56, L"0ab763d2 b142 41d1 af25 765ab16075d2" },
		{ 1955873365,L"e61808d2", IDR_PERSON57, L"39170864 19aa 4483 b8a3 f943eee8e167" },
		{ 919637367,L"d2c0ecb7", IDR_PERSON58, L"a6bb9176 9ed1 4b6f 8f98 b1c45229678e" },
		{ 472410476,L"238475f7", IDR_PERSON59, L"2f48c134 7424 4240 b442 1fb8308f3934" },
		{ 1952585472,L"4336a7e2", IDR_PERSON60, L"f3df1e3e c148 4e4e a2e3 0ad844493fdc" },
		{ 557051467,L"3449cec4", IDR_PERSON61, L"5dc6971f f86e 4247 ae7d f0852cb8ad1c" },
		{ 1653654803,L"ea1cd26f", IDR_PERSON62, L"cc02f668 22f9 43cf b58b 0ce7abee700e" },
		{ 400293138,L"f2573a87", IDR_PERSON63, L"8638587f 487e 47f1 a313 fd5ab0a9fe28" },
		{ 972354977,L"deabce36", IDR_PERSON64, L"1363362b bed8 46c6 b7d4 28cf0784c5bb" },
		{ 129342149,L"0e4a04f2", IDR_PERSON65, L"02e6629b b289 4381 a73a c0f7bb2a8f23" },
		{ 713103924,L"3c8d2dea", IDR_PERSON66, L"914322c6 bdc8 4337 940b 3924302ed844" },
		{ 979411023,L"98b000cd", IDR_PERSON67, L"694f6a1e 34a6 4b5a 9874 44bc218f4c27" },
		{ 487007164,L"1424bc44", IDR_PERSON68, L"c27ea731 34ce 4d0a 9bf0 be61a217753c" },
		{ 949544054,L"02044daa", IDR_PERSON69, L"5e831041 56fb 41f0 9e9e 0fa273bcd379" },
		{ 590611292,L"1be5c700", IDR_PERSON70, L"3981dc2a df46 4e78 88d9 4af6a347e0c6" },
		{ 1646374449,L"1cd2d438", IDR_PERSON71, L"753859a3 17b4 4195 b981 f79b633fe295" },
		{ 333846944,L"f9a68d9f", IDR_PERSON72, L"821b1de8 a65f 48a9 9c8e 783b6b8879e8" },
		{ 1911398856,L"44188ac8", IDR_PERSON73, L"2ec57ad5 0b66 47ba a3e5 a654ebc47f05" },
		{ 221138238,L"4e924155", IDR_PERSON74, L"dc79bb7f 31d4 4cec aebc 651f01f6ee53" },
		{ 253817140,L"dd2728f2", IDR_PERSON75, L"eff6976d 286c 4a2f 8f07 8b8bb83e5993" },
		{ 1616448821,L"55fe8bf2", IDR_PERSON76, L"7b470e36 2a8d 45ee 9030 97b814c80987" },
		{ 680302711,L"465f5b6c", IDR_PERSON77, L"d8a0913a 4810 41e7 bd30 a569120173e5" },
		{ 1185282924,L"05fbe5c3", IDR_PERSON78, L"9778ef6d 1de9 40bb b1ee 558d43294334" },
		{ 1134462462,L"4d7c0f6f", IDR_PERSON79, L"55dd6bf3 f7ac 4457 9a88 912d9a14eb8d" },
		{ 369188835,L"b0ef473e", IDR_PERSON80, L"aa1bafce 4ab8 403c 8c13 ff1dddd3280c" },
		{ 1918298157,L"3e670b9b", IDR_PERSON81, L"7efa5282 3ef5 4694 943e 1b6ea34f23bd" },
		{ 657393004,L"c88f8cc2", IDR_PERSON82, L"d350f17c c775 4cfc b779 b1c29f06359d" },
		{ 2125275279,L"a940c6de", IDR_PERSON83, L"650d8ba4 a2b7 4eea 8f68 6434d7a633cd" },
		{ 600373504,L"28fa4a7c", IDR_PERSON84, L"264edf12 bda0 4636 bbe8 89e4fe8d3ccd" },
		{ 2134793640,L"74054949", IDR_PERSON85, L"3237cc1b 4fe6 4ba0 831c 14232ab2e334" },
		{ 1616514103,L"f9b92233", IDR_PERSON86, L"7c60a966 83dd 4c2b b53d a3ed726652b1" },
		{ 762845357,L"6a1cf1fe", IDR_PERSON87, L"975ca684 25d7 42cc 83ae 4309f3ab7d12" },
		{ 114758392,L"5a4b208a", IDR_PERSON88, L"6f936015 6086 48b4 a63d 3f8db0469ca6" },
		{ 106188527,L"bc3fef2f", IDR_PERSON89, L"482d02bb d549 4222 9731 467c519d1837" },
		{ 964499485,L"b9d00cda", IDR_PERSON90, L"7d81610d 4e1a 4b0b a0ab a212d45ffcd0" },
		{ 1127304913,L"46e388c5", IDR_PERSON91, L"1bb07673 6dbc 4acf b1ea b5af6c414648" },
		{ 67176621,L"d08b95b4", IDR_PERSON92, L"fb598336 f1d7 4893 80f2 0f84df00a0cb" },
		{ 1535295121,L"8d3811e8", IDR_PERSON93, L"241ba0d6 0e82 4ef2 9394 0afdf7d7b12a" },
		{ 39421295,L"fdef8061", IDR_PERSON94, L"f96dd7e2 f22e 4720 8bd2 82fad09a6266" },
		{ 86902976,L"ccce785a", IDR_PERSON95, L"668e4634 6734 4a5a b5d7 99fa564a41c0" },
		{ 354307307,L"7adcda70", IDR_PERSON96, L"b6c049d7 fae6 4ad4 b110 687d24625814" },
		{ 560551905,L"feca79eb", IDR_PERSON97, L"b62f4d12 6a35 4af3 843e 725ffd3bff10" },
		{ 2127773035,L"73b57301", IDR_PERSON98, L"b91c05c6 08ed 4929 9206 b8b70c5cbcc3" },
		{ 91034979,L"5b72ca08", IDR_PERSON99, L"3bdc0727 61ee 4929 9a39 06dcbfb8a706" },
		{ 2090762259,L"549ac965", IDR_PERSON100, L"62411f33 f5d0 47bb bb70 b86b981998c1" },
		{ 1720227784,L"c49f37ac", IDR_PERSON101, L"efc030c5 8629 4fe7 94e2 5624a1042e1f" },
		{ 1334723633,L"b0781f05", IDR_PERSON102, L"9e965eb6 e8f2 498d aad4 f125ba6dca1c" },
		{ 1974947041,L"938d796c", IDR_PERSON103, L"b0aee6f3 d4c4 437b 8269 1b07bcb27305" },
		{ 148273486,L"7e91c965", IDR_PERSON104, L"3714a47e 29e6 4164 833e cce6a74f5ca4" },
		{ 1445062893,L"c48bb0e2", IDR_PERSON105, L"2dd09d75 b477 4558 985d 219f52065ad6" },
		{ 214457230,L"ca4c7793", IDR_PERSON106, L"7ec3051a b330 470a ac23 7b953948c2fe" },
		{ 840705031,L"83a10590", IDR_PERSON107, L"04997479 2eb3 442c 8701 828d439c219b" },
		{ 1735591532,L"a52f2411", IDR_PERSON108, L"7fe16d21 8d4c 4121 a3cf 3841d65d1438" },
		{ 1984406047,L"5c2c0125", IDR_PERSON109, L"fc33502d 5dd8 4f12 96d0 12bcf4ef14ab" },
		{ 1473470959,L"125907ba", IDR_PERSON110, L"0727bb6d ed8f 49fd 93c3 07e946b804d5" },
		{ 1129015673,L"60711244", IDR_PERSON111, L"07ee0969 775d 4d52 9fe7 ea362d51042a" },
		{ 770590441,L"e14c4d2a", IDR_PERSON112, L"91e0d680 4d2b 458d bc86 9537825f2926" },
		{ 1932658552,L"daf6ac6a", IDR_PERSON113, L"5f8a9ce7 e6fd 455b a671 a3752c898d1f" },
		{ 103221641,L"af7f4719", IDR_PERSON114, L"d7332c42 3bf8 4679 95bc a81f7831e838" },
		{ 34287315,L"525f2d66", IDR_PERSON115, L"2d802729 b8ab 4ac3 b67e 033051c6dfe0" },
		{ 2047142110,L"c32cfbd3", IDR_PERSON116, L"e33abd05 a29a 492e b663 f166916225b5" },
		{ 1675863171,L"641b8e1d", IDR_PERSON117, L"b78a9903 61f3 4240 945a e00416f9bc64" },
		{ 1947403281,L"98d65662", IDR_PERSON118, L"a79d202a 7178 4aa2 9086 cf5235a7c71b" },
		{ 985044984,L"4783cb5e", IDR_PERSON119, L"c1eaf579 0967 410a bc71 675fd846a5d4" },
		{ 660311693,L"ff1ff638", IDR_PERSON120, L"2242e525 4847 4588 901e de72fea797ab" },
		{ 2097742214,L"eeea1334", IDR_PERSON121, L"470c3f94 9baf 435f 9ab8 f87c536e1220" },
		{ 864652631,L"990cd71c", IDR_PERSON122, L"d0abd6d5 4739 455e b8b0 472079718fef" },
		{ 380818637,L"a664ef5e", IDR_PERSON123, L"e20cca71 42df 4f57 942c 4b7f91cb8f18" },
		{ 2132528216,L"c06d961b", IDR_PERSON124, L"6886a697 aace 43a1 910d bda476647e62" },
		{ 1610790026,L"a3e732bb", IDR_PERSON125, L"8091f7be c2ed 43fe b4b2 4c04c3f2aede" },
		{ 1579197828,L"d7bfa939", IDR_PERSON126, L"9214fbb9 d351 46f8 9039 e4ca3657e285" },
		{ 946035470,L"0fe22a70", IDR_PERSON127, L"4330a890 5db9 44ee a978 0485b73d3f6c" },
		{ 1871977561,L"7c046326", IDR_PERSON128, L"7b860e42 5031 48e0 b9cc e0a45029f00a" },
		{ 134235262,L"51b51fc5", IDR_PERSON129, L"1996d44d dfc3 49ed a042 421185e93fa8" },
		{ 2046993480,L"e59ce39b", IDR_PERSON130, L"cbc81add 566a 4966 9bd5 23358af787e8" },
		{ 1055896916,L"21edbfd1", IDR_PERSON131, L"563aa51e 00e9 4d06 ab57 7fd61061bfb2" },
		{ 700013323,L"fef97d22", IDR_PERSON132, L"19281cd2 90ff 40db b23a cd66442e8a51" },
		{ 1094247945,L"d8d8f5e1", IDR_PERSON133, L"cde96096 198d 4e2e 9243 92b9bcb9c1d6" },
		{ 1191183850,L"d2d35549", IDR_PERSON134, L"18528c9a 6359 49ee 8788 c85222f6cf0c" },
		{ 796444698,L"33e345bd", IDR_PERSON135, L"23e0b607 7889 4a63 b591 f134d237c438" },
		{ 583574524,L"8c08de96", IDR_PERSON136, L"b694a0a8 ebf0 4278 9602 e5388999a808" },
		{ 829929610,L"fcf38bea", IDR_PERSON137, L"36e5e25b f5be 40ec 9c7e 057b58ca495d" },
		{ 1977001793,L"8e61df0e", IDR_PERSON138, L"12249fdd 0271 4096 9dd0 41e052875b45" },
		{ 1302794258,L"34cde864", IDR_PERSON139, L"dc78e69a 8d9c 409f bd6f 4f6c2545e6d9" },
		{ 1920336410,L"b76ad51c", IDR_PERSON140, L"453c3914 f895 4599 9944 7f593f5708ee" },
		{ 775809072,L"049e8fd4", IDR_PERSON141, L"69c34985 9c02 4815 b164 c99422f8ed36" },
		{ 1174737472,L"2721e23e", IDR_PERSON142, L"4f8f2ac6 3407 4280 9584 a057fe5ae72b" },
		{ 277835992,L"d635906e", IDR_PERSON143, L"b81c897c cd22 42b0 a015 0dca5e2a9efb" },
		{ 780201215,L"a383d385", IDR_PERSON144, L"453f1143 e6f5 482e b8b9 df72f8ae30cf" },
		{ 1982967459,L"fdae0b61", IDR_PERSON145, L"0b5ca02c 0e10 496f a8a3 b297166e0046" },
		{ 356714472,L"b38d22a4", IDR_PERSON146, L"261b9637 8d88 4185 b895 17389f12c3d8" },
		{ 282001716,L"0e33da61", IDR_PERSON147, L"4b0f313e 375a 4b95 8818 e04985f098da" },
		{ 1432073480,L"bbbad94f", IDR_PERSON148, L"3c1a1617 cf1e 4b6c 881f 6f4876714690" },
		{ 5133980,L"992e4988", IDR_PERSON149, L"70039af8 fb9d 48fc 8dbe 2bfdb8bc56e3" },
		{ 187244513,L"4835d76f", IDR_PERSON150, L"d4398c6f 6d7f 4a70 86f7 0f7985658d02" },
		{ 825927783,L"b31f7854", IDR_PERSON151, L"2a53618a d37a 47b7 8d9b ffb2e6a14bda" },
		{ 760632271,L"b7367228", IDR_PERSON152, L"cda792fc 05b1 4fcb 80d4 f6e7f24b4aed" },
		{ 1142728051,L"2f421c35", IDR_PERSON153, L"3a640f86 9264 4b40 85bd 00c9d4e6917c" },
		{ 1578206933,L"266a251a", IDR_PERSON154, L"04b29e7a 8553 430d ae70 8090efc21b24" },
		{ 2140503692,L"6b4329cc", IDR_PERSON155, L"10368a1c 4d80 4130 b534 dcf7014ed30c" },
		{ 855575153,L"943692cb", IDR_PERSON156, L"a0a4c419 f39c 469e a518 5bdc2defd3e9" },
		{ 953904996,L"f6640ba2", IDR_PERSON157, L"8766fc09 2b71 4931 b4a0 165a90628cf3" },
		{ 1989902472,L"6fa924b1", IDR_PERSON158, L"346512e7 7deb 4f61 b058 8df599e665bc" },
		{ 684967107,L"22bc78b0", IDR_PERSON159, L"72219a2c 2ec4 4ef8 94af 8d9cd0e024dd" },
		{ 2013348712,L"700f43d6", IDR_PERSON160, L"2f13f4a0 f198 48a6 bb44 fdefa06e8f04" },
		{ 1415905407,L"06f99ec2", IDR_PERSON161, L"927605c4 b08b 4bdc 98a5 169def23222b" },
		{ 1116211117,L"45618c5d", IDR_PERSON162, L"b6792279 cf71 42ee 8908 967848916c97" },
		{ 1601356270,L"bc17fec5", IDR_PERSON163, L"7e6ef5e6 9a01 4dc1 a78c 5c669f02a808" },
		{ 2084896214,L"3fdb0da1", IDR_PERSON164, L"869c0ab7 d598 4713 bc1b ed6c1a7520ea" },
		{ 417574043,L"9f983842", IDR_PERSON165, L"399c71de 3fe2 4fc3 8a96 df2a9b4bd86e" },
		{ 429002350,L"5333315a", IDR_PERSON166, L"6d70ee9a 127d 4a83 b229 ad404db0f469" },
		{ 1823826143,L"cec77e6e", IDR_PERSON167, L"44377e9b 6aa7 494c a2b1 1ee15ec787b2" },
		{ 741474702,L"e54f6c45", IDR_PERSON168, L"50100d2d 7893 473e a7f0 917b596793f4" },
		{ 1454260590,L"89cf273b", IDR_PERSON169, L"61ae9930 e6d5 42d4 943e 38f2e4491239" },
		{ 1598196438,L"6d4936ef", IDR_PERSON170, L"1b95f372 eedd 494e 93f3 d127a4040762" },
		{ 1217212500,L"239ab13c", IDR_PERSON171, L"05671aa1 dc04 45e4 b462 79705b1b2b2a" },
		{ 1846345025,L"d5f96982", IDR_PERSON172, L"12d6e234 0cd4 4a7a b1a2 a46ee4ef2ee1" },
		{ 644609023,L"c91e21ec", IDR_PERSON173, L"dccaae1e b70f 49bc a921 95aa72e7ffde" },
		{ 1212192221,L"47c18a5d", IDR_PERSON174, L"bba27947 b976 4c04 adc7 7984b9edd525" },
		{ 2031986268,L"289606cc", IDR_PERSON175, L"4a0500bd 94f7 4742 a9fc 49542ea7a63f" },
		{ 923004742,L"f1530a87", IDR_PERSON176, L"740be4f0 ddae 47b5 a773 110d2430045e" },
		{ 586816639,L"d1110197", IDR_PERSON177, L"85786665 230e 4f14 a4ad 4608a9d186d0" },
		{ 1748101069,L"3c2e74e3", IDR_PERSON178, L"b5b9d98e 0012 4c0d 8eac c16ef73ed30a" },
		{ 149560757,L"cfcad273", IDR_PERSON179, L"2801d523 5a8d 4ed0 9725 9bcb9d7bf17b" },
		{ 1254075554,L"13866a97", IDR_PERSON180, L"d5fd4138 6c4d 44c4 81c5 a2b7ba42932b" },
		{ 1297196112,L"14c8a63f", IDR_PERSON181, L"6f47fcc8 cf10 487b 9ef4 3c4ed8810d79" },
		{ 1661445637,L"ac206d86", IDR_PERSON182, L"b2817488 e429 486f a869 9d5d16341d18" },
		{ 1866843581,L"04deafd0", IDR_PERSON183, L"66d68248 e9c3 445d 9992 299a30e9475c" },
		{ 2094474396,L"5ec75443", IDR_PERSON184, L"75d3e9a3 3117 4083 b9d4 77030338b2b3" },
		{ 1221065697,L"df441897", IDR_PERSON185, L"82798552 c26d 414f a07d 175408836462" },
		{ 295264645,L"a7d9ffbd", IDR_PERSON186, L"61a4ec10 4973 437f 9e38 4d104e7cf3de" },
		{ 1704768919,L"80e215ec", IDR_PERSON187, L"1cccc742 1898 402b a2f7 8f6a82cb581e" },
		{ 1663524659,L"536ac7c1", IDR_PERSON188, L"34121c44 875f 42e5 8837 18ecd95ca3c6" },
		{ 1198163805,L"09d6a513", IDR_PERSON189, L"26b237ca 2574 41e8 bd3c 76c173d608a0" },
		{ 2088353192,L"2aa01a00", IDR_PERSON190, L"f3d050c7 d84b 468b 8b5a db44e5214a24" },
		{ 1777153175,L"c02a1f94", IDR_PERSON191, L"48aba4e7 b6f4 4f55 ba5c 9dd9218c6e98" },
		{ 987510785,L"4a53ad5b", IDR_PERSON192, L"2a2d2df9 b8c3 4108 8dc8 0694922c7445" },
		{ 1292034686,L"c0fd7f95", IDR_PERSON193, L"433c239e e3e7 4b53 a147 4560683b644b" },
		{ 1436929193,L"3e8e1eae", IDR_PERSON194, L"555df253 557a 4466 bd20 33a355588391" },
		{ 504431003,L"c744ce6e", IDR_PERSON195, L"8c939536 d26c 40cc ba0d b57a1a31030a" },
		{ 1807081602,L"8d49a4ee", IDR_PERSON196, L"5fa0a859 545c 4f85 9016 bd7e3f1480f7" },
		{ 1720864849,L"516255a6", IDR_PERSON197, L"6bf0319c e8cf 434d 9963 9701617d23d6" },
		{ 340423425,L"714204d8", IDR_PERSON198, L"6bfef786 9f81 491d b223 312036078c35" },
		{ 362627172,L"29e94db7", IDR_PERSON199, L"952e3ece 85c3 425e 806d 03c87154cac6" },
	};
	return result;
}