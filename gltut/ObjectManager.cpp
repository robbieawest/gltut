
namespace ObjectManager { //Diabolical fix
	class ObjectManager {
		static ObjectManager* instance;
	};
}

ObjectManager::ObjectManager* ObjectManager::ObjectManager::instance = nullptr;
