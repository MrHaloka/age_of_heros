#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RTSPlayerState.generated.h"

enum class EAge;
enum class ETeams;
/**
 * 
 */
UCLASS()
class AOE2_API ARTSPlayerState : public APlayerState
{
	GENERATED_BODY()
	void SetPlayerTeam(ETeams NewPlayerTeam);
	void SetPlayerScore(int32 NewScore);
	void SetPlayerAge(EAge NewAge);
protected:
	ETeams PlayerTeam;
	int32 PlayerScore;
	EAge PlayerAge;
};
