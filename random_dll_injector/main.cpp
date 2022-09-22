#include "includes.h"
#include "functions.h"

void Cleanup(const std::string message) {
	std::cout << message << std::endl;
	system("pause");
	ExitProcess(0);
}

std::string RandomString(const size_t length) {
	std::string r;
	static const char bet[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYabcdefghijklmnopqrstuvwxyzZ1234567890" };
	srand((unsigned)time(NULL) * 5);
	for (int i = 0; i < length; ++i)
		r += bet[rand() % (sizeof(bet) - 1)];
	return r;
}

int main() {
	SetConsoleTitleA(RandomString(26).c_str());

	DWORD ProcessId = Functions::GetProcessId("csgo.exe");

	if (!ProcessId)
		Cleanup("No Process Found");

	HANDLE game = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
	uintptr_t ModuleBase = Functions::GetModuleBaseAddress(ProcessId, "client.dll");

	std::cout << "csgo.exe --> " << std::dec << ProcessId << std::endl;

	if (Functions::DoesFileExist("hack.dll")) {
		if (!Functions::Internal::ExecuteBypass(game)) {
			Cleanup("Cannot Bypass ");
		}

		if (Functions::LoadLibraryInject(ProcessId, "hack.dll")) {
			Functions::Internal::Backup(game);
			std::cout << "Injected" << std::endl;
			system("pause");
			ExitProcess(0);
		}
		else
		{
			Functions::Internal::Backup(game);
			Cleanup("Injection Failed ");
		}
	}

}