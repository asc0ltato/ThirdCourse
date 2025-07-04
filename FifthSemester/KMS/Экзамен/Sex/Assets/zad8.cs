using UnityEngine;
using UnityEngine.EventSystems;

public class zad8 : MonoBehaviour, IPointerClickHandler {
	private Renderer rend;
	void Start () {
        rend = GetComponent<Renderer>();
	}
	
	public void OnPointerClick (PointerEventData eventData) {
        rend.material.color = Color.red;
	}
}
