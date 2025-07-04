using UnityEngine;

public class zad10 : MonoBehaviour //12pract
{
    void Update()
    {
        float x = Input.GetAxis("Mouse X"); 
        float y = Input.GetAxis("Mouse Y"); 
        transform.Rotate(x, y, 0);
    }
}