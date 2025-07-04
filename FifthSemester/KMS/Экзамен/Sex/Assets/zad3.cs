using UnityEngine;

public class zad3 : MonoBehaviour {
	[SerializeField]
	Transform centralObject;
	int sensivity = 3;

	void Update () {
		if (Input.GetMouseButton(1))
		{
			transform.RotateAround(centralObject.position, Vector3.up, Input.GetAxis("Mouse X") * sensivity);
		}
	}
}