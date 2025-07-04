using UnityEngine;

public class zad16 : MonoBehaviour {
	public GameObject panel;

	void Start () {
		panel.SetActive(false);
	}
	
	public void ShowPanel () {
		panel.SetActive (true);
	}
}