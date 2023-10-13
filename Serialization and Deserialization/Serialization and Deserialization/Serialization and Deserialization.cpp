#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream> 
#include <memory>


class ListNode
{
public:
	std::shared_ptr <ListNode> Previous;
	std::shared_ptr <ListNode> Next;
	std::shared_ptr<ListNode> Rand;
	std::string Data;

	ListNode()
	{
		Previous = nullptr;
		Next = nullptr;
		Rand = nullptr;
	}
};

class ListRand
{
public:
	std::shared_ptr <ListNode> Head;
	std::shared_ptr <ListNode> Tail;
	int Count;

	void Serialize(std::ofstream& s)
	{
		std::unordered_map<std::shared_ptr<ListNode>, int> IndexMap;
		int Index = 0;
		std::shared_ptr<ListNode> Current = Head;

		if (Current != nullptr)
		{
			while (Current)
			{
				IndexMap[Current] = Index;
				Index++;
				Current = Current->Next;
			}
		}
		//s.seekp(0);


		Current = Head;
		if (Current != nullptr)
		{
			while (Current)
			{
				int PreviousIndex = (Current->Previous) ? IndexMap[Current->Previous] : -1;
				int RandIndex = (Current->Rand) ? IndexMap[Current->Rand] : -1;
				s << Current->Data << " " << PreviousIndex << " " << RandIndex << std::endl;

				Current = Current->Next;
			}
		}
	}

	void Deserialize(std::ifstream& s)
	{
		std::unordered_map<int, std::shared_ptr<ListNode>> IndexMap;
		std::string line;
		int index = 0;

		while (getline(s, line))
		{
			std::istringstream InputStringStream(line);
			std::string Data;
			int PreviousIndex, RandIndex;

			InputStringStream >> Data >> PreviousIndex >> RandIndex;

			std::shared_ptr<ListNode> NewNode = std::make_shared<ListNode>();
			NewNode->Data = Data;

			IndexMap[index] = NewNode;
			if (PreviousIndex != -1)
			{
				NewNode->Previous = IndexMap[PreviousIndex];
			}
			if (RandIndex != -1)
			{
				NewNode->Rand = IndexMap[RandIndex];
			}
			
			if (index == 0)
			{
				Head = NewNode;
				Tail = NewNode;
			}
			
			else
			{
				Tail->Next = NewNode;
				NewNode->Previous = Tail;
				Tail = NewNode;
			}
			index++;
		}
		Count = index;
	}
};


void PrintList(const ListRand& list)
{
	std::shared_ptr<ListNode> current = list.Head;
	while (current)
	{
		std::cout << "Data: " << current->Data << " ";
		if (current->Previous) {
			std::cout << "Prev: " << current->Previous->Data << " ";
		}
		if (current->Rand) {
			std::cout << "Rand: " << current->Rand->Data << " ";
		}
		std::cout << std::endl;
		current = current->Next;
	}
}

int main()
{
	ListRand list;

	// Добавляем в список элементы
	auto node1 = std::make_shared<ListNode>();
	auto node2 = std::make_shared<ListNode>();
	auto node3 = std::make_shared<ListNode>();
	auto node4 = std::make_shared<ListNode>();
	auto node5 = std::make_shared<ListNode>();

	node1->Data = "A";
	node2->Data = "B";
	node3->Data = "C";
	node4->Data = "D";
	node5->Data = "E";

	node1->Next = node2;
	node2->Next = node3;
	node3->Next = node4;
	node4->Next = node5;

	node2->Previous = node1;
	node3->Previous = node2;
	node4->Previous = node3;
	node5->Previous = node4;

	list.Head = node1;
	list.Tail = node5;
	list.Count = 5;

	node2->Rand = node2;
	node3->Rand = node5;
	node4->Rand = node1;

	//Сериализируем 
	std::ofstream outFile("serialized_list.txt");
	list.Serialize(outFile);
	outFile.close();

	//Десеарилизируем
	std::ifstream inFile("serialized_list.txt");
	ListRand deserializedList;
	deserializedList.Deserialize(inFile);
	inFile.close();

	PrintList(deserializedList);

	return 0;
}