using UnityEngine;

public class zad6 : MonoBehaviour { //10pract
	float speed = 0.05f;
	
	void Update () {
		transform.rotation *= Quaternion.AngleAxis(30f * speed, new Vector3(1,1,1));
	}
}