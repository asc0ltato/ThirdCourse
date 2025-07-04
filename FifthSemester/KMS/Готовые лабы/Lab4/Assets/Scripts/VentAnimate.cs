using UnityEngine;
using UnityEngine.EventSystems;

public class VentAnimate : MonoBehaviour, IPointerClickHandler
{
    Animator anim;
    private bool isAnimating = false;

    void Start()
    {
        anim = GetComponent<Animator>();  
    }

    public void OnPointerClick(PointerEventData eventData)
    {
        isAnimating = !isAnimating;

        if (isAnimating)
        {
            anim.SetTrigger("hittenOn");  
        }
        else
        {
            anim.SetTrigger("hittenOff");  
        }
    }
}