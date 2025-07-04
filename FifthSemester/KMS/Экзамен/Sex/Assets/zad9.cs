using UnityEngine;

public class zad9 : MonoBehaviour {
	float speed = 1.0f;

	void Update () {
		if(Input.GetKey(KeyCode.E))
		{
			transform.Rotate(Vector3.right * speed);
		}
        if (Input.GetKey(KeyCode.R))
        {
            transform.Rotate(Vector3.up * speed);
        }
        if (Input.GetKey(KeyCode.T))
        {
            transform.Rotate(Vector3.forward * speed);
        }
    }
}
