#include "turnbased/TurnEngine.h"

TurnEngine::TurnEngine(YOUR_CHARACTER_CLASS* player)
{
	this->playerCharacter = player;
	this->characterStack.Push(player);
	this->currentCharacterIndex = 0;
	this->currentCharacter = player;
	this->waitingForCharacter = false;
	this->spawned = false;
	this->turnCount = 0;
	this->setPhase(TurnPhase::PHASE_Decision);
}

TurnEngine::~TurnEngine()
{
	this->clearStack();
}

bool TurnEngine::Tick(float deltaseconds)
{
	if (this->characterStack.Num() != 0) {
		switch (phase)
		{
			case TurnPhase::PHASE_Decision:
			{
				if (!this->waitingForCharacter) {
					this->currentCharacter->beginMakeDecision();
					this->waitingForCharacter = true;
				}

				bool decisionMade = this->currentCharacter->makeDecision(deltaseconds);
				if (this->currentCharacter->getHealth() < 1)
					this->setNextCharacter();

				if (decisionMade) {
					this->setPhase(TurnPhase::PHASE_Action);
				}

				break;
			}

			case TurnPhase::PHASE_Action:
			{
				if (!this->waitingForCharacter) {
					this->currentCharacter->beginMakeDecision();
					this->waitingForCharacter = true;
				}

				bool actionMade = this->currentCharacter->executeAction(deltaseconds);

				if (actionMade) {
					this->setNextCharacter();
					this->setPhase(TurnPhase::PHASE_Decision);
				}

				break;
			}

			case TurnPhase::PHASE_Loss:
			{
				return true;
				break;
			}
		}
	}


	return false;

}

void TurnEngine::addToStack(YOUR_CHARACTER_CLASS* character)
{
	characterStack.Push(character);
}

void TurnEngine::removeFromStack(YOUR_CHARACTER_CLASS* character)
{
	characterStack.Remove(character);
}

void TurnEngine::sortBySpeed()
{
// search up a sort algo on youtube and apply here, only if u want to
}

void TurnEngine::setPhase(TurnPhase next_phase)
{
	this->phase = next_phase;
}

void TurnEngine::setNextCharacter()
{
	if (playerCharacter->getHealth() <= 0) {
		this->setPhase(TurnPhase::PHASE_Loss);
		return;
	}
	this->waitingForCharacter = false;
	uint8 deadcount = 0;
	if (this->currentCharacterIndex >= characterStack.Num() - 1) {
		this->currentCharacterIndex = 0;
		this->turnCount++;
		this->spawned = false;
		this->currentCharacter = this->characterStack[0];
	}
	else {
		for (auto i = this->currentCharacterIndex; i < characterStack.Num(); i++) {
			if (this->characterStack[i] == this->currentCharacter) {
				continue;
			}
				if (this->characterStack[i]->getHealth() < 1) {
					deadcount++;
				}
				else {
					this->currentCharacter = this->characterStack[i];
					this->currentCharacterIndex = i;
					break;
				}

		}
	}
}

void TurnEngine::clearStack()
{
  // Destroy the actors first
	for (auto character : this->characterStack) {
		if (character != this->playerCharacter)
			character->Destroy();
	}
	// then delete the pointers
	characterStack.Empty();
}
