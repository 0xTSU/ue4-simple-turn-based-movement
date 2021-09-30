#pragma once

#include "YOUR_CHARACTER_CLASS.h"
#include "Kismet/GameplayStatics.h"
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class TurnPhase : uint8
{
  // Logic, math, error correction in this state
	PHASE_Decision,
  // Playing animation in this state
	PHASE_Action,
  // when player health reaches 0
	PHASE_Loss
};

class GAME_API TurnEngine
{
// VARIABLES
protected:

  // Main character list
  // Enemies are added and removed as they are spawned/defeated
	TArray<YOUR_CHARACTER_CLASS*> characterStack;
	
  // Waits for player
	bool waitingForCharacter;

  // You
	YOUR_CHARACTER_CLASS* playerCharacter;
  
  // Current element in list
	YOUR_CHARACTER_CLASS* currentCharacter;
	
  // current element index in list
	int8 currentCharacterIndex;
	
  // current turn
	int64 turnCount;
	
  // phase
	TurnPhase phase;

// FUNCTIONS
public:

  // called when PHASE_LOSS detected
	void clearStack();

  // constructor
  // when level is loaded, playerstart spawns the player pawn and sets player character
  // Pass in player character reference(in gamemode blueprint) when level is loaded
  // This initiates turn based movement
	TurnEngine(YOUR_CHARACTER_CLASS* player);

  // to clear out list
	~TurnEngine();
  
  int getTurnCount() { return turnCount; };

	YOUR_CHARACTER_CLASS* getCurrentCharacter() { return currentCharacter; };
  
  YOUR_CHARACTER_CLASS* getPlayerCharacter() { return playerCharacter; };

	int getCharacterCount() { return characterStack.Num(); };

	void addToStack(YOUR_CHARACTER_CLASS* character);

	void removeFromStack(YOUR_CHARACTER_CLASS* character);

	TurnPhase getPhase() { return phase; };

  // to be called in gamemode tick function
  // returns false when gamemode is in its loss phase.
	bool Tick(float DeltaTime);

  // for character spawner (not included)
	bool spawned;

protected:
  // called when decision is made, animation is completed, or playerhealth reaches 0
	void setPhase(TurnPhase next_phase);

  // if you have a speed stat implemented, when the list reaches the end
  // the list is sorted by the character's speed stat
	void sortBySpeed();

  // called when character's ACTION phase ends
	void setNextCharacter();

};

/*
0. Level and gamemode are loaded
1. Player start spawns player pawn
2. Have something like this in the gamemode's source file

UFUNCTION(BlueprintCallable, Category = "Turn Engine")
void createDungeonInstance(YOUR_PLAYER_CHARACTER * player);

void createDungeonInstance(YOUR_PLAYER_CHARACTER * player) {
  turn_engine_instance = new TurnEngine(player); 
}

3. TurnEngine is instantiated in memory. Constructor is ran. Playercharacter is added to characterStack list

4. Have something like this in the gamemode's tick function

void Tick(float delta)
{
  if (this->turn_engine_instance != nullptr) {
    bool combating = this->turn_engine_instance->Tick(delta);
    
    if (!combating)
      //stuff goes here

  }
}

5. Implement an enemy spawner.
6. The enemy's BeginPlay() needs to get the current gamemode and add their reference to the characterStack using addToStack().
7. Proper turn based features!

*/
