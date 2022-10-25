// ILikeBanas


#include "FSBuildableService.h"
#include "FSkyline.h"
#include "Blueprint/WidgetLayoutLibrary.h"


void UFSBuildableService::Init()
{
	AFSkyline* FSkyline = AFSkyline::Get(this);

	this->World = FSkyline->World;
	this->FGController = FSkyline->FGController;
	//this->Player = FSkyline->Player;
	this->BuildableSubsystem = AFGBuildableSubsystem::Get(this);

	Current = &BuildableSet1;
	Backup = &BuildableSet2;
	/*
	BuildableCache.Empty();
	for (TActorIterator<AFGBuildable>It(World); It; ++It) {
		AFGBuildable* Buildable = *It;
		Current->Add(Buildable);
	}

	FScriptDelegate Func;
	Func.BindUFunction(this, FName("onBuildableCreated"));
	this->BuildableSubsystem->BuildableConstructedGlobalDelegate.Add(Func);
	*/
	srand(FSTime::Current());
}

void UFSBuildableService::Update()
{
	Backup->Empty();
	for (BuildablePtr Ptr : *Current) if (Ptr.Get()) {
		Backup->Add(Ptr);
	}
	for (BuildablePtr Ptr : BuildableCache) if (Ptr.Get()) {
		Backup->Add(Ptr);
	}
	BuildableCache.Empty();
	Swap(Current, Backup);
	Backup->Empty();

	//SML::Logging::info(Current->Num());
	StaticTree.BuildTree(*Current);
	//SML::Logging::info(TEXT("Finish"));
}

void UFSBuildableService::onBuildableCreated(AFGBuildable* Buildable)
{
	BuildableCache.Add(Buildable);
}


void KDTree::Partition(TArray<BuildablePtr>& Buildables, TArray<Float3D>& Pos, int d, int k, int& Start, int& End)
{
	int s = Start + Random() % (End - Start);
	Swap(Buildables[Start], Buildables[s]);
	Swap(Pos[Start], Pos[s]);

	s = Start;
	int e = End-1;
	float value = Pos[s].Pos[d];

	while (s<e) {
		if (Pos[e].Pos[d] < Pos[s].Pos[d]) {
			Swap(Buildables[s], Buildables[e]);
			Swap(Pos[s], Pos[e]);
		}
		if (Pos[s].Pos[d] < value) s++;
		else e--;
	}

	if (s <= k) {
		e = s + 1;
		for (int i = e; i < End; i++) if (Pos[i].Pos[d] == value) {
			Swap(Buildables[i], Buildables[e]);
			Swap(Pos[i], Pos[e]);
			e++;
		}
		if (k < e) {
			Start = s;
			End = e;
			return;
		}
		Start = e;
		Partition(Buildables, Pos, d, k, Start, End);
	}
	else {
		End = s;
		Partition(Buildables, Pos, d, k, Start, End);
	}
}

void KDTree::BuildTree(TArray<BuildablePtr>& Buildables, TArray<Float3D>& Pos, int NodeIndex, int D, int Start, int End)
{
	//SML::Logging::info(TEXT("Node "), NodeIndex, TEXT(" ["), Start, TEXT(" "), End, TEXT(" ]"));
	KDTreeNode Node;

	for (int d = 0; d < MaxDim; d++) {
		int Dim = (d + D >= MaxDim) ? d + D - MaxDim : d + D;
		int s = Start;
		int e = End;
		Partition(Buildables, Pos, Dim, (Start + End) / 2, s, e);

		int mid;
		if (s - Start > End - e) mid = s;
		else mid = e;

		//SML::Logging::info(Dim, TEXT(" "), s, TEXT(" "), e, TEXT(" "), mid);

		if (mid - Start >= MinElementCount && End - mid >= MinElementCount) {
			Node.Dim = Dim;
			Node.Split = Pos[mid].Pos[Dim];

			Node.Left = Tree.Add(KDTreeNode());
			Node.Right = Tree.Add(KDTreeNode());
			//SML::Logging::info(Node.Left, TEXT(" "), Node.Right, TEXT(" splited part: ["), Start, TEXT(" "), mid, TEXT(" ], [ "), mid, TEXT(" "), End, TEXT(" ]"));
			
			BuildTree(Buildables, Pos, Node.Left, Dim + 1, Start, mid);
			BuildTree(Buildables, Pos, Node.Right, Dim + 1, mid, End);
			//SML::Logging::info(TEXT("Node "), NodeIndex, TEXT(" splited part: ["), Start, TEXT(" "), mid, TEXT(" ], [ "), mid, TEXT(" "), End, TEXT(" ]"));
			Tree[NodeIndex] = Node;
			return;
		}
	}

	Tree[NodeIndex] = Node;
	TArray<BuildablePtr>& BuildableArray = Tree[NodeIndex].Buildables;
	for (int i = Start; i < End; i++) if (Buildables[i].Get()) BuildableArray.Add(Buildables[i]);

	//SML::Logging::info(TEXT("Node "), NodeIndex, TEXT(" stored "), Tree[NodeIndex].Buildables.Num(), TEXT(" buildables."));
}

void KDTree::BuildTree(const TSet<BuildablePtr>& BuildableSet)
{
	TArray<BuildablePtr> Buildables;
	TArray<Float3D> Pos;
	for (BuildablePtr Ptr: BuildableSet) {
		AFGBuildable* Buildable = Ptr.Get();
		if (Buildable) {
			Buildables.Add(Ptr);
			Pos.Add(Float3D(Buildable->GetTransform().GetLocation()));
			//SML::Logging::info(TEXT("Add "), *Buildable->GetTransform().GetLocation().ToString());
		}
	}

	Tree.Empty();
	Tree.Add(KDTreeNode());
	//SML::Logging::info(TEXT("BuildTree"));
	BuildTree(Buildables, Pos, 0, 0, 0, Buildables.Num());
}

AFGBuildable* KDTree::QueryNearest(const FVector& Vector, float& Dist)
{
	Float3D Pos(Vector);
	AFGBuildable* Result = nullptr;
	if (Tree.Num()) QueryNearest(Pos, Dist, Result, 0);
	return Result;
}

void KDTree::QueryNearest(const Float3D& Pos, float& Dist, AFGBuildable*& Buildable, int NodeIndex)
{
	KDTreeNode& Node = Tree[NodeIndex];
	if (!Node.Left || !Node.Right) {
		for (BuildablePtr Ptr : Node.Buildables) if (Ptr.Get()) {
			FVector Vector(Ptr.Get()->GetTransform().GetLocation());
			float DistSqr = Sqrf(Pos.Pos[0] - Vector.X) + Sqrf(Pos.Pos[1] - Vector.Y) + Sqrf(Pos.Pos[2] - Vector.Z);
			//SML::Logging::info(TEXT("Query "), *Vector.ToString(), TEXT(" "), Pos.Pos[0], TEXT(" "), Pos.Pos[1], TEXT(" "), Pos.Pos[2], TEXT(" "), DistSqr);
			if (DistSqr < Sqrf(Dist)) {
				Buildable = Ptr.Get();
				Dist = sqrtf(DistSqr);
			}
		}
		return;
	}

	if (Node.Left && Pos.Pos[Node.Dim] - Dist < Node.Split)	QueryNearest(Pos, Dist, Buildable, Node.Left);
	if (Node.Right && !(Pos.Pos[Node.Dim] + Dist < Node.Split))	QueryNearest(Pos, Dist, Buildable, Node.Right);
}


void KDTree2D::BuildTree(const TSet<TWeakObjectPtr<AFGBuildable>>& BuildableSet, APlayerController * Player)
{
	FVector2D ScreenPosition;
	FVector2D ScreenSize = UWidgetLayoutLibrary::GetViewportSize(Player);
	SizeX = ScreenSize.X + 0.5;
	SizeY = ScreenSize.Y + 0.5;

	Data.Empty();
	Tree.Empty();
	for (TWeakObjectPtr<AFGBuildable> Buildable : BuildableSet) if (Buildable.Get())
		if (UGameplayStatics::ProjectWorldToScreen(Player, Buildable.Get()->GetActorLocation(), ScreenPosition)) {
			if (Reset) return;
			int x = ScreenPosition.X + 0.5;
			int y = ScreenPosition.Y + 0.5;
			if (0 <= x && x < SizeX && 0 <= y && y < SizeY) {
				KDTreeDataType2D& Entry = Data.AddDefaulted_GetRef();
				Entry.Buildable = Buildable;
				Entry.Loc.Pos[0] = x;
				Entry.Loc.Pos[1] = y;
			}
		}

	Int2D Box[2];
	Box[0].Pos[0] = Box[0].Pos[1] = 0;
	Box[1].Pos[0] = SizeX;
	Box[1].Pos[1] = SizeY;

	Tree.AddDefaulted();

	BuildTree(0, 0, Data.Num(), Box);
}

void KDTree2D::QueryDelta(TArray<TWeakObjectPtr<AFGBuildable> >& PositiveParam, TArray<TWeakObjectPtr<AFGBuildable> >& NagetiveParam, const Int2D& StartOld, const Int2D& EndOld, const Int2D& StartNew, const Int2D& EndNew)
{
	this->Positive = &PositiveParam;
	this->Nagetive = &NagetiveParam;

	Int2D Box[6];
	Box[0].Pos[0] = Box[0].Pos[1] = 0;
	Box[1].Pos[0] = SizeX;
	Box[1].Pos[1] = SizeY;
	Box[2] = StartOld;
	Box[3] = EndOld;
	Box[4] = StartNew;
	Box[5] = EndNew;

	QueryDelta(0, Box);
}

void KDTree2D::BuildTree(int NodeIndex, int Start, int End, Int2D* Box)
{
	if (Reset) return;

	int x, y, d, mid;
	x = Box[1].Pos[0] - Box[0].Pos[0];
	y = Box[1].Pos[1] - Box[0].Pos[1];
	Tree[NodeIndex].IndexStart = Start;
	Tree[NodeIndex].IndexEnd = End;
	if (Start == End) return;
	if (x == 1 && y == 1) return;

	d = (x >= y) ? 0 : 1;
	mid = (Box[1].Pos[d] + Box[0].Pos[d]) / 2;

	int s = Start;
	int e = End - 1;
	while (s <= e) {
		if (Reset) return;

		if (Data[s].Loc.Pos[d] < mid) s++;
		else if (Data[e].Loc.Pos[d] < mid) {
			Swap(Data[s], Data[e]);
			s++;
		}
		else e--;
	}
	
	int left = 0;
	int right = 0;
	Int2D Child[2];
	Child[0] = Box[0];
	Child[1] = Box[1];
	if (Start < s) {
		left = Tree.AddDefaulted();
		Child[1].Pos[d] = mid;
		BuildTree(left, Start, s, Child);
		Child[1] = Box[1];
	}
	if (s < End) {
		right = Tree.AddDefaulted();
		Child[0].Pos[d] = mid;
		BuildTree(right, s, End, Child);
	}

	Tree[NodeIndex].Dim = d;
	Tree[NodeIndex].Split = mid;
	Tree[NodeIndex].Left = left;
	Tree[NodeIndex].Right = right;
}

void KDTree2D::QueryDelta(int NodeIndex, Int2D* Box)
{
	if (Reset) return;
	KDTreeNode2D& Node = Tree[NodeIndex];
	if (Node.IndexStart == Node.IndexEnd) return;

	if (Box[3].Pos[0] <= Box[0].Pos[0] || Box[1].Pos[0] <= Box[2].Pos[0] || Box[3].Pos[1] <= Box[0].Pos[1] || Box[1].Pos[1] <= Box[2].Pos[1]) {
		if (Box[4].Pos[0] <= Box[0].Pos[0] && Box[1].Pos[0] <= Box[5].Pos[0] && Box[4].Pos[1] <= Box[0].Pos[1] && Box[1].Pos[1] <= Box[5].Pos[1]) {
			for (int i = Node.IndexStart; i < Node.IndexEnd; i++) {
				if (Reset) return;
				Positive->Add(Data[i].Buildable);
			}
			return;
		}
		if (Box[5].Pos[0] <= Box[0].Pos[0] || Box[1].Pos[0] <= Box[4].Pos[0] || Box[5].Pos[1] <= Box[0].Pos[1] || Box[1].Pos[1] <= Box[4].Pos[1])
			return;
	}
	if (Box[2].Pos[0] <= Box[0].Pos[0] && Box[1].Pos[0] <= Box[3].Pos[0] && Box[2].Pos[1] <= Box[0].Pos[1] && Box[1].Pos[1] <= Box[3].Pos[1]) {
		if (Box[5].Pos[0] <= Box[0].Pos[0] || Box[1].Pos[0] <= Box[4].Pos[0] || Box[5].Pos[1] <= Box[0].Pos[1] || Box[1].Pos[1] <= Box[4].Pos[1]) {
			for (int i = Node.IndexStart; i < Node.IndexEnd; i++) {
				if (Reset) return;
				Nagetive->Add(Data[i].Buildable);
			}
			return;
		}
		if (Box[4].Pos[0] <= Box[0].Pos[0] && Box[1].Pos[0] <= Box[5].Pos[0] && Box[4].Pos[1] <= Box[0].Pos[1] && Box[1].Pos[1] <= Box[5].Pos[1])
			return;
	}

	int tmp;
	if (Node.Left) {
		tmp = Box[1].Pos[Node.Dim];
		Box[1].Pos[Node.Dim] = Node.Split;
		QueryDelta(Node.Left, Box);
		Box[1].Pos[Node.Dim] = tmp;
	}
	if (Node.Right) {
		tmp = Box[0].Pos[Node.Dim];
		Box[0].Pos[Node.Dim] = Node.Split;
		QueryDelta(Node.Right, Box);
		Box[0].Pos[Node.Dim] = tmp;
	}
}
