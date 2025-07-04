using UnityEngine;

public class zad5 : MonoBehaviour {
	float speed = 0.05f;
	
	void Update () {
		transform.Translate(new Vector3(1,1,1) * speed);
		transform.Rotate(new Vector3(60,0,0) * speed);
	}
}
