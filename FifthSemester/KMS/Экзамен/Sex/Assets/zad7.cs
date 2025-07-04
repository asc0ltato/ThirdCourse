using UnityEngine;

public class zad7 : MonoBehaviour { //13pract
	public GameObject prefab;
	
	void Update () {
		if (Input.GetKeyDown(KeyCode.Space))
		{
			Vector3 pos = new Vector3(Random.Range(-10f,10f), Random.Range(-10f, 10f), Random.Range(-10f, 10f));
			Instantiate(prefab, pos, Quaternion.identity);
		}
	}
}
