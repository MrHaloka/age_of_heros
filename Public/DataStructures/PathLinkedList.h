#pragma once
#include "CoreMinimal.h"
#include "PathLinkedList.generated.h"

USTRUCT()
struct FPathLinkedList
{
	GENERATED_BODY()

protected:
	FVector2d Path;
	FPathLinkedList* Next;
	FPathLinkedList* Previous;
public:
	FPathLinkedList()
	{
		Path = FVector2d::Zero();
		Next = nullptr;
		Previous = nullptr;
	}

	FPathLinkedList(const FVector2d& NewPath)
	{
		Path = NewPath;
		Next = nullptr;
		Previous = nullptr;
	}

	FPathLinkedList* GetNext() const
	{
		return Next;
	}

	FPathLinkedList* GetPrevious() const
	{
		return Previous;
	}

	void SetNext(FPathLinkedList* NewNext)
	{
		Next = NewNext;
	}

	void SetPrevious(FPathLinkedList* NewPrevious)
	{
		Previous = NewPrevious;
	}

	FVector2d GetPath()
	{
		return Path;
	}

	void AddAfter(FPathLinkedList* After)
	{
		After->SetNext(this->GetNext());
		if (this->GetNext() != nullptr)
		{
			this->GetNext()->SetPrevious(After);
		}
		this->SetNext(After);
		After->SetPrevious(this);
	}

	void AddBefore(FPathLinkedList* Before)
	{
		Before->SetPrevious(this->GetPrevious());
		if (this->GetPrevious() != nullptr)
		{
			this->GetPrevious()->SetNext(Before);
		}
		this->SetPrevious(Before);
		Before->SetNext(this);
	}

	void SetPath(const FVector2d& NewPath)
	{
		Path = NewPath;
	}

	void Replace(FPathLinkedList* Replace)
	{
		this->SetNext(Replace->GetNext());
		this->SetPrevious(Replace->GetPrevious());
		this->SetPath(Replace->GetPath());
	}

	void RemoveAndReconnect()
	{
		if (this->GetNext() != nullptr)
		{
			this->GetNext()->SetPrevious(this->GetPrevious());
		}
		if (this->GetPrevious() != nullptr)
		{
			this->GetPrevious()->SetNext(this->GetNext());
		}
	}

	void DeleteList()
	{
		FPathLinkedList* Current = this->Previous;
		while (Current != nullptr)
		{
			auto OldPath = Current;
			Current = Current->GetPrevious();
			delete OldPath;
		}
		delete this;
	}
};
