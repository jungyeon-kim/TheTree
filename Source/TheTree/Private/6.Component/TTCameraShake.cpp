#include "TTCameraShake.h"

UTTCameraShake::UTTCameraShake()
{
	OscillationDuration = 0.2f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(0.5f, 1.0f);
	RotOscillation.Pitch.Frequency = FMath::RandRange(50.0f, 70.0f);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(0.5f, 1.0f);
	RotOscillation.Yaw.Frequency = FMath::RandRange(50.0f, 70.0f);
}