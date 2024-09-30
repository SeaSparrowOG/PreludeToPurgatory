#include "papyrus.h"

#include "defaultObjects.h"

namespace Papyrus {
    std::vector<int> GetVersion(STATIC_ARGS) {
        std::vector<int> response = std::vector<int>();
        response.push_back(Version::MAJOR);
        response.push_back(Version::MINOR);
        response.push_back(Version::PATCH);

        return response;
    }

    std::vector<RE::Actor*> FetchNearbyCorpses(STATIC_ARGS) {
        _loggerDebug("Called fetch nearby corpses");
        auto response = std::vector<RE::Actor*>();
        auto* noReanimateKeyword = DefaultObjects::ModObject<RE::BGSKeyword>("MagicNoReanimate"sv);
        if (!noReanimateKeyword) return response;

        if (const auto TES = RE::TES::GetSingleton()) {
            TES->ForEachReferenceInRange(RE::PlayerCharacter::GetSingleton()->AsReference(), 1500.0f, [&](RE::TESObjectREFR* a_ref) {
                if (a_ref && a_ref->Is3DLoaded()) {
                    if (auto* actorRef = a_ref->As<RE::Actor>()) {
                        if (actorRef->IsDead() && !actorRef->HasKeyword(noReanimateKeyword)) {
                            response.push_back(actorRef);
                            _loggerDebug("  Pushing back {}", actorRef->GetName());
                        }
                    }
                }
            return RE::BSContainer::ForEachResult::kContinue;
            });
        }
        _loggerInfo("  Returning response");
        return response;
    }

    bool Bind(VM& a_vm)
    {
        BIND(GetVersion);
        BIND(FetchNearbyCorpses);
        return true;
    }

    bool RegisterFunctions(VM* a_vm) {
        Bind(*a_vm);
        return true;
    }
}