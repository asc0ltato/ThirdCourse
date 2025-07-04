using UnityEngine;

public class zad4 : MonoBehaviour {
	bool move = false;
	float speed = 0.01f;
	float offset = 0f;
	Vector3 startPosition;
	Vector3 needPosition;
	Quaternion startRotation;
	Quaternion needRotation;

	public void Move()
	{
		move = true;
		startPosition = transform.position;
		startRotation = transform.rotation;
		needPosition = new Vector3(-0.07f, 0.77f, -2.97f);
		needRotation = Quaternion.AngleAxis(30, new Vector3(1,0,0));

	}
	void Update () {
		if (move) {
			offset += speed;
			transform.position = Vector3.Lerp(startPosition, needPosition, offset);
			transform.rotation = Quaternion.Slerp(startRotation, needRotation, offset);

			if (offset >= 1)
			{
				move = false;
				offset = 0f;
			}
		}
	}
}