#pragma once

class STUUtils
{
public:
	template <typename CompnentType>
	static CompnentType* GetSTUPlayerComponent(const AActor* Pawn)
	{
		return Pawn ? Cast<CompnentType>(Pawn->GetComponentByClass(CompnentType::StaticClass())) : nullptr;
	}
};