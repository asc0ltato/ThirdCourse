using System.Collections;
using UnityEngine;

public class pract17 : MonoBehaviour {
	Color color;
	Renderer rend;

	void Start () {
		rend = GetComponent<Renderer>();
		color= rend.material.color;
		StartCoroutine(changeColor());
	}

	private IEnumerator changeColor()
	{
		rend.material.color = Color.black;
		yield return new WaitForSeconds(2f);
		rend.material.color = color;
	}
}