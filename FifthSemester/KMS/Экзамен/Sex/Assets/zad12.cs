using UnityEngine;

public class zad12 : MonoBehaviour { //14pract,15
    float speed = 0.1f;
    Renderer rend;
    Color color;

    void Start()
    {
        rend = GetComponent<Renderer>();
        color = rend.material.color;
    }
    void Update()
    {
        float x = Input.GetAxis("Horizontal") * speed;
        float z = Input.GetAxis("Vertical") * speed;
        transform.Translate(x, 0, z);
    }

    void OnTriggerEnter(Collider col)
    {
        this.GetComponent<Renderer>().material.color = Color.red;
    }

    void OnTriggerExit(Collider col)
    {
        rend.material.color = color;
    }

    //15
    void OnTriggerStay(Collider col)
    {
        Debug.Log(gameObject.name + " находится в триггере");
    }
}
