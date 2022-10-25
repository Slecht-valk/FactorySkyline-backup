// ILikeBanas


#include "FSAdvancedCopyPanel.h"
#include "FactorySkyline/FSkyline.h"
#include "Windows/WindowsPlatformApplicationMisc.h"
#include <sstream>

UFSAdvancedCopyPanel::UFSAdvancedCopyPanel(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFSAdvancedCopyPanel::onCheckBoxChanged(bool IsChecked)
{
	if (IsChecked) this->Expand();
	else this->Fold();
}

void UFSAdvancedCopyPanel::onCheckBoxChangedVector(bool IsChecked)
{
	if (IsChecked && this->FoldPanel->FoldBox->IsChecked()) {
		VectorXBox->SetVisibility(ESlateVisibility::Visible);
		VectorYBox->SetVisibility(ESlateVisibility::Visible);
		VectorZBox->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		VectorXBox->SetVisibility(ESlateVisibility::Collapsed);
		VectorYBox->SetVisibility(ESlateVisibility::Collapsed);
		VectorZBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UFSAdvancedCopyPanel::onCheckBoxChangedRotation(bool IsChecked)
{
	if (IsChecked && this->FoldPanel->FoldBox->IsChecked()) {
		RotationXBox->SetVisibility(ESlateVisibility::Visible);
		RotationYBox->SetVisibility(ESlateVisibility::Visible);
		RotationZBox->SetVisibility(ESlateVisibility::Visible);
		RotationWBox->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		RotationXBox->SetVisibility(ESlateVisibility::Collapsed);
		RotationYBox->SetVisibility(ESlateVisibility::Collapsed);
		RotationZBox->SetVisibility(ESlateVisibility::Collapsed);
		RotationWBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UFSAdvancedCopyPanel::onCheckBoxChangedCircle(bool IsChecked)
{
	if (IsChecked && this->FoldPanel->FoldBox->IsChecked()) {
		RotationAngleBox->SetVisibility(ESlateVisibility::Visible);
		SetCirclePanel->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		RotationAngleBox->SetVisibility(ESlateVisibility::Collapsed);
		SetCirclePanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UFSAdvancedCopyPanel::OnEditableTextCommittedVectorX(const FText& Text, ETextCommit::Type CommitMethod)
{
	double Value = FCString::Atod(*Text.ToString());
	FText NewText = FText::FromString(Format(Value));
	VectorX->SetText(NewText);
	UpdateTransform();
}

void UFSAdvancedCopyPanel::OnEditableTextCommittedVectorY(const FText& Text, ETextCommit::Type CommitMethod)
{
	double Value = FCString::Atod(*Text.ToString());
	FText NewText = FText::FromString(Format(Value));
	VectorY->SetText(NewText);
	UpdateTransform();
}

void UFSAdvancedCopyPanel::OnEditableTextCommittedVectorZ(const FText& Text, ETextCommit::Type CommitMethod)
{
	double Value = FCString::Atod(*Text.ToString());
	FText NewText = FText::FromString(Format(Value));
	VectorZ->SetText(NewText);
	UpdateTransform();
}

void UFSAdvancedCopyPanel::OnEditableTextCommittedRotationX(const FText& Text, ETextCommit::Type CommitMethod)
{
	double Value = FCString::Atod(*Text.ToString());
	FText NewText = FText::FromString(Format(Value));
	RotationX->SetText(NewText);
	UpdateTransform();
}

void UFSAdvancedCopyPanel::OnEditableTextCommittedRotationY(const FText& Text, ETextCommit::Type CommitMethod)
{
	double Value = FCString::Atod(*Text.ToString());
	FText NewText = FText::FromString(Format(Value));
	RotationY->SetText(NewText);
	UpdateTransform();
}

void UFSAdvancedCopyPanel::OnEditableTextCommittedRotationZ(const FText& Text, ETextCommit::Type CommitMethod)
{
	double Value = FCString::Atod(*Text.ToString());
	FText NewText = FText::FromString(Format(Value));
	RotationZ->SetText(NewText);
	UpdateTransform();
}

void UFSAdvancedCopyPanel::OnEditableTextCommittedRotationW(const FText& Text, ETextCommit::Type CommitMethod)
{
	double Value = FCString::Atod(*Text.ToString());
	FText NewText = FText::FromString(Format(Value));
	RotationW->SetText(NewText);
	UpdateTransform();
}

void UFSAdvancedCopyPanel::OnEditableTextCommittedRotationAngle(const FText& Text, ETextCommit::Type CommitMethod)
{
	double Value = FCString::Atod(*Text.ToString());
	FText NewText = FText::FromString(Format(Value));
	RotationAngle->SetText(NewText);
	UpdateCircle();
}

FString UFSAdvancedCopyPanel::Format(double Value)
{
	std::stringstream buf;
	buf.precision(8);
	buf << Value;
	std::string str = buf.str();

	size_t offset = str.find('.');
	if (offset == std::string::npos || str.length() <= offset + 9)
		return FString(str.c_str());

	std::stringstream buf2;
	buf2.precision(8);
	buf2.setf(std::ios::fixed);
	buf2 << Value;

	return FString(buf2.str().c_str());
}

void UFSAdvancedCopyPanel::onBuild()
{
	AFSkyline* Skyline = AFSkyline::Get(this);
	if (Skyline->FSCtrl->CheckAnchor(true)) {
		Skyline->FSCtrl->StartCopyMode();
		Skyline->FSCtrl->BuildOne = true;
		Skyline->SkylineUI->BackToSelectMapping->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UFSAdvancedCopyPanel::onSetRepeat()
{
	AFSkyline* Skyline = AFSkyline::Get(this);
	if (Skyline->FSCtrl->CheckAnchor(true)) {
		Skyline->FSCtrl->StartCopyMode();
		Skyline->FSCtrl->CopyPosition = true;
		Skyline->SkylineUI->CopyMapping->SetVisibility(ESlateVisibility::Collapsed);
		Skyline->SkylineUI->BackToSelectMapping->SetVisibility(ESlateVisibility::Collapsed);
		Skyline->SkylineUI->SetRepeatMapping->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UFSAdvancedCopyPanel::onSetCircle()
{
	AFSkyline* Skyline = AFSkyline::Get(this);
	if (Skyline->FSCtrl->Repeat.Ready)
		Skyline->FSCtrl->StartSetItemMode();
}

void UFSAdvancedCopyPanel::onClipboard()
{
	FString Value = FString::Printf(TEXT("Vector={X=%s, Y=%s, Z=%s}\nRotation={X=%s, Y=%s, Z=%s, W=%s}\n"), *VectorX->GetText().ToString(), *VectorY->GetText().ToString(), *VectorZ->GetText().ToString(),
		*RotationX->GetText().ToString(), *RotationY->GetText().ToString(), *RotationZ->GetText().ToString(), *RotationW->GetText().ToString());

	FWindowsPlatformApplicationMisc::ClipboardCopy(*Value);
}

void UFSAdvancedCopyPanel::SetCircleFeedback()
{
	AFSkyline* Skyline = AFSkyline::Get(this);
	CircleCenter = Skyline->FSCtrl->CurrentFocusBuilding;
	if (!CircleCenter.Get()) return;
	if (!Skyline->FSCtrl->Repeat.Ready) return;

	if (Skyline->FSCtrl->CheckAnchor(true)) {
		Skyline->FSCtrl->StartCopyMode();
		Skyline->SkylineUI->OperatingHelper->SetVisibility(ESlateVisibility::Hidden);
		Skyline->SkylineUI->RepeatWidget->Load();
		Skyline->SkylineUI->RepeatWidget->StartEnterAnimation();
		Skyline->FSCtrl->CopyConsole = true;
		Skyline->FSCtrl->ShowMouseCursor();

		UpdateCircle();
	}
}

void UFSAdvancedCopyPanel::UpdateCircle()
{
	AFSkyline* Skyline = AFSkyline::Get(this);
	if (!CircleCenter.Get()) return;

	FSRepeat& Repeat = Skyline->FSCtrl->Repeat;
	if (!Repeat.Ready) return;

	float Angle = FCString::Atof(*RotationAngle->GetText().ToString());

	FVector Center = CircleCenter.Get()->GetActorLocation();
	FQuat Rotation = FQuat(FVector(0.0f, 0.0f, 1.0f), FMath::DegreesToRadians<float>(Angle));
	FTransform Transform(Rotation, FVector::ZeroVector);
	FTransform NewTarget(Transform.TransformRotation(Repeat.Target.GetRotation()),
		Transform.TransformPosition(Repeat.Target.GetLocation() - Center) + Center);
		
	SetTransform(FTransform(Repeat.Target.InverseTransformRotation(NewTarget.GetRotation()),
		Repeat.Target.InverseTransformPosition(NewTarget.GetLocation())));
		
	UpdateTransform();
}

void UFSAdvancedCopyPanel::Init()
{
	FScriptDelegate Func;
	Func.BindUFunction(this, FName("onCheckBoxChanged"));
	this->FoldPanel->FoldBox->OnCheckStateChanged.Add(Func);
	Func.BindUFunction(this, FName("onCheckBoxChangedVector"));
	this->CheckBoxVector->OnCheckStateChanged.Add(Func);
	Func.BindUFunction(this, FName("onCheckBoxChangedRotation"));
	this->CheckBoxRotation->OnCheckStateChanged.Add(Func);
	Func.BindUFunction(this, FName("onCheckBoxChangedCircle"));
	this->CheckBoxCircle->OnCheckStateChanged.Add(Func);

	Func.BindUFunction(this, FName("onBuild"));
	this->BuildButton->OnClicked.Add(Func);

	Func.BindUFunction(this, FName("onSetRepeat"));
	this->SetRepeatButton->OnClicked.Add(Func);

	Func.BindUFunction(this, FName("onClipboard"));
	this->ClipboardButton->OnClicked.Add(Func);

	Func.BindUFunction(this, FName("onSetCircle"));
	this->SetCircleButton->OnClicked.Add(Func);

	Func.BindUFunction(this, FName("OnEditableTextCommittedVectorX"));
	this->VectorX->OnTextCommitted.Add(Func);
	Func.BindUFunction(this, FName("OnEditableTextCommittedVectorY"));
	this->VectorY->OnTextCommitted.Add(Func);
	Func.BindUFunction(this, FName("OnEditableTextCommittedVectorZ"));
	this->VectorZ->OnTextCommitted.Add(Func);
	Func.BindUFunction(this, FName("OnEditableTextCommittedRotationX"));
	this->RotationX->OnTextCommitted.Add(Func);
	Func.BindUFunction(this, FName("OnEditableTextCommittedRotationY"));
	this->RotationY->OnTextCommitted.Add(Func);
	Func.BindUFunction(this, FName("OnEditableTextCommittedRotationZ"));
	this->RotationZ->OnTextCommitted.Add(Func);
	Func.BindUFunction(this, FName("OnEditableTextCommittedRotationW"));
	this->RotationW->OnTextCommitted.Add(Func);
	Func.BindUFunction(this, FName("OnEditableTextCommittedRotationAngle"));
	this->RotationAngle->OnTextCommitted.Add(Func);

	this->FoldPanel->Title->SetText(FText::FromString(FString(TEXT("Repeat Copy"))));

	this->FoldPanel->FoldBox->SetCheckedState(ECheckBoxState::Unchecked);
	this->onCheckBoxChanged(false);
	this->CheckBoxVector->SetCheckedState(ECheckBoxState::Unchecked);
	this->onCheckBoxChangedVector(false);
	this->CheckBoxRotation->SetCheckedState(ECheckBoxState::Unchecked);
	this->onCheckBoxChangedRotation(false);
	this->CheckBoxCircle->SetCheckedState(ECheckBoxState::Unchecked);
	this->onCheckBoxChangedCircle(false);
}

void UFSAdvancedCopyPanel::Load(UFSDesign* Design)
{
	this->Design = Design;
	this->SetTransform(FTransform(FRotator::ZeroRotator, FVector::ZeroVector));
}

void UFSAdvancedCopyPanel::UpdateTransform()
{
	AFSkyline* Skyline = AFSkyline::Get(this);
	if (Skyline->FSCtrl->CopyConsole) {
		Skyline->FSCtrl->Repeat.NextRelative = GetTransform();
		Skyline->Builder->Update(&Skyline->FSCtrl->Repeat);
	}
}

FTransform UFSAdvancedCopyPanel::GetTransform()
{
	FVector Vector;
	Vector.X = FCString::Atof(*VectorX->GetText().ToString());
	Vector.Y = FCString::Atof(*VectorY->GetText().ToString());
	Vector.Z = FCString::Atof(*VectorZ->GetText().ToString());

	FQuat Quat;
	Quat.X = FCString::Atof(*RotationX->GetText().ToString());
	Quat.Y = FCString::Atof(*RotationY->GetText().ToString());
	Quat.Z = FCString::Atof(*RotationZ->GetText().ToString());
	Quat.W = FCString::Atof(*RotationW->GetText().ToString());
	
	return FTransform(Quat, Vector);
}

void UFSAdvancedCopyPanel::SetTransform(const FTransform& Transform)
{
	VectorX->SetText(FText::FromString(Format(Transform.GetLocation().X)));
	VectorY->SetText(FText::FromString(Format(Transform.GetLocation().Y)));
	VectorZ->SetText(FText::FromString(Format(Transform.GetLocation().Z)));

	FQuat Quat = Transform.GetRotation();
	RotationX->SetText(FText::FromString(Format(Quat.X)));
	RotationY->SetText(FText::FromString(Format(Quat.Y)));
	RotationZ->SetText(FText::FromString(Format(Quat.Z)));
	RotationW->SetText(FText::FromString(Format(Quat.W)));
}

void UFSAdvancedCopyPanel::Fold()
{
	this->FoldPanel->Dark->SetVisibility(ESlateVisibility::Collapsed);
	this->BuildPanel->SetVisibility(ESlateVisibility::Collapsed);
	this->SetRepeatPanel->SetVisibility(ESlateVisibility::Collapsed);
	this->ClipboardPanel->SetVisibility(ESlateVisibility::Collapsed);
	this->CheckBoxVector->SetVisibility(ESlateVisibility::Collapsed);
	this->CheckBoxRotation->SetVisibility(ESlateVisibility::Collapsed);
	this->CheckBoxCircle->SetVisibility(ESlateVisibility::Collapsed);
	this->BuildStatus->SetVisibility(ESlateVisibility::Collapsed);

	this->onCheckBoxChangedVector(CheckBoxVector->IsChecked());
	this->onCheckBoxChangedRotation(CheckBoxRotation->IsChecked());
	this->onCheckBoxChangedCircle(CheckBoxCircle->IsChecked());
}

void UFSAdvancedCopyPanel::Expand()
{
	this->FoldPanel->Dark->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->BuildPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->SetRepeatPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->ClipboardPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->CheckBoxVector->SetVisibility(ESlateVisibility::Visible);
	this->CheckBoxRotation->SetVisibility(ESlateVisibility::Visible);
	this->CheckBoxCircle->SetVisibility(ESlateVisibility::Visible);
	//this->BuildStatus->SetVisibility(ESlateVisibility::Visible);

	this->onCheckBoxChangedVector(CheckBoxVector->IsChecked());
	this->onCheckBoxChangedRotation(CheckBoxRotation->IsChecked());
	this->onCheckBoxChangedCircle(CheckBoxCircle->IsChecked());
}
