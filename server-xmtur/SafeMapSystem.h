
class CSafeMapSystem
{
public:
	void StartProcess();
	void LoadConfigs();
	BOOL SafeCheck(int aIndex, int tIndex);

private:
	int Enabled;
	int SafeMap[MAX_NUMBER_MAP];
	int MaxResetDiference;
};

extern CSafeMapSystem SafeMap;

