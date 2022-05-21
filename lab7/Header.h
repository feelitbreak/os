class MonitorStack
{
private:
	short* mass;
	int n;
	int size;
public:
	MonitorStack(int nSize);
	~MonitorStack();
	void Push(short& nElement);
	char Pop();
};

struct ElementsToProduce
{
private:
	short* massElem;
	int nElem;
public:
	ElementsToProduce(int n, short* mass) {
		nElem = n;
		massElem = mass;
	}

};