
namespace Resource { //Diabolical fix
	class ResourceManager {
		static ResourceManager* instance;
	};
}

Resource::ResourceManager* Resource::ResourceManager::instance = nullptr;
