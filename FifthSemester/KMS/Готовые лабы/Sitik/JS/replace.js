document.querySelectorAll('.image-replace').forEach(function (span) {
    span.addEventListener('mouseover', function () {
        const img = span.querySelector('img');
        img.style.display = 'block';
    });
    span.addEventListener('mouseout', function () {
        const img = span.querySelector('img');
        img.style.display = 'none';
    });
});