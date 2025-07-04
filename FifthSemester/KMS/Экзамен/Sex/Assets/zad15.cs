using UnityEngine;
using UnityEngine.EventSystems;

public class zad15 : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler { //20pract
	[SerializeField]
    new GameObject gameObject;
    Renderer rend;
	Color color;

	void Start()
	{
		rend = gameObject.GetComponent<Renderer>();
		color = rend.material.color;
	}
	public void OnPointerEnter (PointerEventData eventData)
	{
		rend.material.color = Color.gray;
	}
    public void OnPointerExit (PointerEventData eventData)
    {
        rend.material.color = color;
    }
}