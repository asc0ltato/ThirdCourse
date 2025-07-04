using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class ButtonHandler : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler
{
    public Text infoText; 
    public string elementDescription; 
    private Color highlightColor = new Color(1, 0, 0);
    private Color originalColor;
    private Material originalMaterial;
    public GameObject elementObject;

    private void Start()
    {
        if (elementObject != null)
        {
            Renderer renderer = elementObject.GetComponent<Renderer>();
            if (renderer != null)
            {
                originalColor = renderer.material.color; 
                originalMaterial = renderer.material;   
            }
        }
    }

    public void OnPointerEnter(PointerEventData eventData)
    {
        if (infoText != null)
        {
            infoText.text = elementDescription;
        }

        if (elementObject != null)
        {
            Renderer renderer = elementObject.GetComponent<Renderer>();
            if (renderer != null)
            {
                renderer.material.color = highlightColor;
            }
        }

    }

    public void OnPointerExit(PointerEventData eventData)
    {
        if (infoText != null)
        {
            infoText.text = "";
        }

        if (elementObject != null)
        {
            Renderer renderer = elementObject.GetComponent<Renderer>();
            if (renderer != null)
            {
                renderer.material.color = originalColor; 
                renderer.material = originalMaterial;  
            }
        }
    }
}