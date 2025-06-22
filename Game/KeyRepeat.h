#pragma once

// �ȈՃL�[���s�[�g�N���X
class KeyRepeat
{
private:

	// �L�[���s�[�g�̊Ԋu
	static constexpr int KEY_REPEAT_INTEVAL = 10;

public:

	// �ȈՃL�[���s�[�g
	static int GetKey(int keyCondition)
	{
		// �L�[�������ꂽ�t���[����
		static int elapseFrame = 0;
		// �P�t���[���O�̃L�[���
		static int oldKey = 0;

		int keyTrigger = ~oldKey & keyCondition;
		int keyRepeat = 0;

		// �L�[��������Ă���
		if (keyCondition)
		{
			// �L�[�������ꂽ�u�Ԃ̓L�[���𔽉f����
			if (keyTrigger)
			{
				keyRepeat = keyCondition;
				elapseFrame = 0;
			}
			// ���s�[�g�Ԋu�𒴂���
			if (++elapseFrame >= KEY_REPEAT_INTEVAL)
			{
				elapseFrame = 0;
				keyRepeat = keyCondition;
			}
		}

		oldKey = keyCondition;

		return keyRepeat;
	}
};